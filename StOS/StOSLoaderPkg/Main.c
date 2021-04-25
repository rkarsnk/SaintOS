#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/BlockIo.h>
#include <Guid/FileInfo.h>

// \kernel.elf を含む12文字
#define LEN_OF_KERNFILENAME 12
#define KERN_BASE_ADDR 0x100000;

#define UEFI_PAGE_SIZE 0x1000

#define ENTRY_POINT_OFFSET 24

/* MemoryMap structure */
struct MemoryMap{
  UINTN buffer_size;
  VOID *buffer;
  UINTN map_size;
  UINTN map_key;
  UINTN descriptor_size;
  UINT32 descriptor_version;
};

/* GetMemoryMap */
EFI_STATUS GetMemoryMap(struct MemoryMap *map){
  if (map->buffer == NULL){
    return EFI_BUFFER_TOO_SMALL;
  }

  map->map_size = map->buffer_size;
  return gBS->GetMemoryMap(
      &map->map_size,
      (EFI_MEMORY_DESCRIPTOR *)map->buffer,
      &map->map_key,
      &map->descriptor_size,
      &map->descriptor_version);
}

const CHAR16 *GetMemoryTypeUnicode(EFI_MEMORY_TYPE type){
  switch (type){
  case EfiReservedMemoryType:
    return L"EfiReservedMemoryType";
  case EfiLoaderCode:
    return L"EfiLoaderCode";
  case EfiLoaderData:
    return L"EfiLoaderData";
  case EfiBootServicesCode:
    return L"EfiBootServicesCode";
  case EfiBootServicesData:
    return L"EfiBootServicesData";
  case EfiRuntimeServicesCode:
    return L"EfiRuntimeServicesCode";
  case EfiRuntimeServicesData:
    return L"EfiRuntimeServicesData";
  case EfiConventionalMemory:
    return L"EfiConventionalMemory";
  case EfiUnusableMemory:
    return L"EfiUnusableMemory";
  case EfiACPIReclaimMemory:
    return L"EfiACPIReclaimMemory";
  case EfiACPIMemoryNVS:
    return L"EfiACPIMemoryNVS";
  case EfiMemoryMappedIO:
    return L"EfiMemoryMappedIO";
  case EfiMemoryMappedIOPortSpace:
    return L"EfiMemoryMappedIOPortSpace";
  case EfiPalCode:
    return L"EfiPalCode";
  case EfiPersistentMemory:
    return L"EfiPersistentMemory";
  case EfiMaxMemoryType:
    return L"EfiMaxMemoryType";
  default:
    return L"InvalidMemoryType";
  }
}

/* SaveMemoryMap */
EFI_STATUS SaveMemoryMap(struct MemoryMap *map, EFI_FILE_PROTOCOL *file){
  CHAR8 buf[256];
  UINTN len;

  CHAR8 *header = "Index, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
  len = AsciiStrLen(header);
  //ヘッダをファイルの先頭に書き出す
  file->Write(file, &len, header);

  Print(L"[INFO] map->buffer = %08lx, map->map_size = %08lx\n", map->buffer, map->map_size);

  EFI_PHYSICAL_ADDRESS iter;
  int idx;
  for (iter = (EFI_PHYSICAL_ADDRESS)map->buffer, idx = 0;
       iter < (EFI_PHYSICAL_ADDRESS)map->buffer + map->map_size;
       iter += map->descriptor_size, idx++)
  {
    EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)iter;
    len = AsciiSPrint(
        buf, sizeof(buf),
        "%u, %x, %-ls, %08lx, %lx, %lx\n",
        idx, desc->Type, GetMemoryTypeUnicode(desc->Type), desc->PhysicalStart,
        desc->NumberOfPages, desc->Attribute & 0xffffflu);
    file->Write(file, &len, buf);
  }

  return EFI_SUCCESS;
}

/* OpenRootDir */
EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL **root)
{
  EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

  gBS->OpenProtocol(
      image_handle,
      &gEfiLoadedImageProtocolGuid,
      (VOID **)&loaded_image,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  gBS->OpenProtocol(
      loaded_image->DeviceHandle,
      &gEfiSimpleFileSystemProtocolGuid,
      (VOID **)&fs,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  fs->OpenVolume(fs, root);

  return EFI_SUCCESS;
}

/* UefiMain */
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *system_table)
{
  EFI_TIME time;
  gRT->GetTime(&time, NULL);
  Print(L"Hello, St.OS Loader World! %4u-%02u-%2u \n",time.Year, time.Month, time.Day);

  /* メモリマップの取得 */
  CHAR8 memmap_buffer[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buffer), memmap_buffer, 0, 0, 0, 0};
  GetMemoryMap(&memmap);

  /* メモリマップのファイルへの書き出し */
  EFI_FILE_PROTOCOL* root_dir;
  OpenRootDir(image_handle, &root_dir);

  EFI_FILE_PROTOCOL *memmap_file;
  root_dir->Open(
      root_dir, &memmap_file, L"\\memmap",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);

  SaveMemoryMap(&memmap, memmap_file);
  memmap_file->Close(memmap_file);

  /* kernel.elfの読み込み */
  EFI_FILE_PROTOCOL* kernel_file;
  // EFIファイルシステムをOpenし、kernel.elfを読み出す
  root_dir->Open(
      root_dir, &kernel_file, L"\\kernel.elf",
      EFI_FILE_MODE_READ, 0);

  // file_info_size はEFI_FILE_INFOのサイズ＋ファイル名長(ヌル文字含む)
  UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * LEN_OF_KERNFILENAME;
  UINT8 file_info_buffer[file_info_size];  

  /* kernel.elfファイルの情報取得 */

  // file_info_bufferにEFI_FILE_INFO型のデータが書き込まれる
  kernel_file->GetInfo(
    kernel_file, &gEfiFileInfoGuid, 
    &file_info_size, file_info_buffer);

  EFI_FILE_INFO* kernel_file_info = (EFI_FILE_INFO*)file_info_buffer;
  UINTN kernel_file_size = kernel_file_info->FileSize;

  EFI_PHYSICAL_ADDRESS kernel_base_addr = KERN_BASE_ADDR;
  // AllocatePages()の第３引数はページ数。UEFIでの1ページは4KiB。
  // kernel_file_sizeが4KiBの倍数とは限らないため、ページ数を切り上げるために
  // kernel_file_sizeに4095を足している
  // ページ数 = ( kernel_file_size + 4095 ) / 4096
  gBS->AllocatePages(
    AllocateAddress, EfiLoaderData,
    (kernel_file_size + 0xfff) / UEFI_PAGE_SIZE, &kernel_base_addr);
  kernel_file->Read(kernel_file, &kernel_file_size, (VOID*)kernel_base_addr);
  Print(L"Kernel: 0x%0lx (%lu bytes)\n", kernel_base_addr, kernel_file_size);

  /* EFI BootServiceの終了 */
  EFI_STATUS status;
  status = gBS->ExitBootServices(image_handle, memmap.map_key);
  if (EFI_ERROR(status)){
    status = GetMemoryMap(&memmap);
    if (EFI_ERROR(status)){
      Print(L"failed to get memory map: %r\n", status);
      while(1);
    }
    status=gBS->ExitBootServices(image_handle, memmap.map_key);
    if(EFI_ERROR(status)){
      Print(L"Could not exit boot service: %r\n", status);
      while(1);
    }
  }

  // kernelの呼び出し
  UINT64 entry_addr = *(UINT64*)(kernel_base_addr + ENTRY_POINT_OFFSET);

  typedef void EntryPointType(void);
  EntryPointType* entry_point = (EntryPointType*)entry_addr;
  entry_point();

  Print(L"All done\n");

  while (1);
  return EFI_SUCCESS;
}
