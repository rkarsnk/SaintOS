#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/BlockIo.h>
#include <Guid/FileInfo.h>

#include "Include/FrameBufferConfig.hpp"
#include "Include/ElfBinaryInfo.hpp"
#include "Include/MemoryMap.h"

// \kernel.elf を含む12文字
#define LEN_OF_KERNFILENAME 12
// kernel base address
#define KERN_BASE_ADDR 0x100000;
// UEFI Page size 4KiB
#define UEFI_PAGE_SIZE 0x1000
// ELF形式のENTRYPOINT Offset
#define ENTRY_POINT_OFFSET 24
// FrameBuffer Color white
#define FRAME_BUFFER_COLOR 255

#define INFO 0
#define ERROR 1

void Stall(){
  // 0.1秒処理を遅らせる
  gBS->Stall(100000);
}

/* 実装途中
void PrintMessage(UINTN level, const CHAR16* Format){
  switch (level){
  case INFO:
    Print(L"[");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGREEN);
    Print(L"INFO");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    Print(L"] ");
    Print(Format);
    Stall();
    break;
  
  case ERROR:
    Print(L"[");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
    Print(L"ERROR");
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
    Print(L"] ");
    Print(Format);
    Stall();
    break;
  }
}
*/

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

const CHAR16* GetMemoryTypeUnicode(EFI_MEMORY_TYPE type){
  switch (type){
  case EfiReservedMemoryType: return L"EfiReservedMemoryType";
  case EfiLoaderCode: return L"EfiLoaderCode";
  case EfiLoaderData: return L"EfiLoaderData";
  case EfiBootServicesCode: return L"EfiBootServicesCode";
  case EfiBootServicesData: return L"EfiBootServicesData";
  case EfiRuntimeServicesCode: return L"EfiRuntimeServicesCode";
  case EfiRuntimeServicesData: return L"EfiRuntimeServicesData";
  case EfiConventionalMemory: return L"EfiConventionalMemory";
  case EfiUnusableMemory: return L"EfiUnusableMemory";
  case EfiACPIReclaimMemory: return L"EfiACPIReclaimMemory";
  case EfiACPIMemoryNVS: return L"EfiACPIMemoryNVS";
  case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
  case EfiMemoryMappedIOPortSpace: return L"EfiMemoryMappedIOPortSpace";
  case EfiPalCode: return L"EfiPalCode";
  case EfiPersistentMemory: return L"EfiPersistentMemory";
  case EfiMaxMemoryType: return L"EfiMaxMemoryType";
  default: return L"InvalidMemoryType";
  }
}

/* SaveMemoryMap */
EFI_STATUS SaveMemoryMap( struct MemoryMap *map, 
                          EFI_FILE_PROTOCOL *file){
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
       iter += map->descriptor_size, idx++){
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
EFI_STATUS OpenRootDir( EFI_HANDLE image_handle, 
                        EFI_FILE_PROTOCOL **root){
  EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

  gBS->OpenProtocol(
      image_handle,
      &gEfiLoadedImageProtocolGuid,
      (VOID **)&loaded_image,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  Print(L"[INFO] UEFI Image Base :0x%0lx\n",loaded_image->ImageBase);

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

/* OpenGOP GOP:Graphics Output Protocol */
EFI_STATUS OpenGOP( EFI_HANDLE image_handle, 
                    EFI_GRAPHICS_OUTPUT_PROTOCOL** gop){
  UINTN num_gop_handles = 0;
  EFI_HANDLE* gop_handles = NULL;
  gBS->LocateHandleBuffer(
    ByProtocol,
    &gEfiGraphicsOutputProtocolGuid,
    NULL,
    &num_gop_handles,
    &gop_handles);
  
  gBS->OpenProtocol(
    gop_handles[0],
    &gEfiGraphicsOutputProtocolGuid,
    (VOID**)gop,
    image_handle,
    NULL,
    EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  FreePool(gop_handles);

  return EFI_SUCCESS;
}

/* GetPixelFormatUnicode */
const CHAR16* GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT fmt){
  switch (fmt) {
    case PixelRedGreenBlueReserved8BitPerColor: return L"PixelRedGreenBlueReserved8BitPerColor";
    case PixelBlueGreenRedReserved8BitPerColor: return L"PixelBlueGreenRedReserved8BitPerColor";
    case PixelBitMask: return L"PixelBitMask";
    case PixelBltOnly: return L"PixelBltOnly";
    case PixelFormatMax: return L"PixelFormatMax";
    default: return L"InvalidPixelFormat";
  }
}

/* CalcLoadAddressRange */
void CalcLoadAddressRange(Elf64_ElfHeader* ehdr, UINT64* head, UINT64* tail){
  Elf64_ProgramHeader* phdr = (Elf64_ProgramHeader*)((UINT64)ehdr + ehdr->e_phoffset );
  *head = MAX_UINT64;
  *tail = 0;
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i ) {
    if( phdr[i].p_type != PT_LOAD) continue;
    *head = MIN(*head, phdr[i].p_vaddr);
    *tail = MAX(*tail, phdr[i].p_vaddr + phdr[i].p_memsz);
  }
}

/* CopyLoadSegments */
void CopyLoadSegments(Elf64_ElfHeader* ehdr){
  Elf64_ProgramHeader* phdr = (Elf64_ProgramHeader*)((UINT64)ehdr + ehdr->e_phoffset);
  for(Elf64_Half i = 0 ; i < ehdr->e_phnum; ++i ) {
    if( phdr[i].p_type != PT_LOAD ) continue;

    UINT64 segm_in_file = (UINT64)ehdr + phdr[i].p_offset;
    CopyMem((VOID*)phdr[i].p_vaddr, (VOID*)segm_in_file, phdr[i].p_filesz);

    UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz;
    SetMem((VOID*)(phdr[i].p_vaddr + phdr[i].p_filesz), remain_bytes, 0);
  }
}


/* Halt function */
void Halt(void) {
  while (1) __asm__("hlt");
}

/*----------------------------------------------
 キー入力があるまで待ち、キー入力があれば返す関数
 ※https://github.com/No000/WcatOS の実装を参考
-----------------------------------------------*/
EFI_STATUS WaitForPressAnyKey(){
  EFI_STATUS status;

  Print(L"Press any key to continue:\n");

  status = gBS->WaitForEvent(1, &(gST->ConIn->WaitForKey), 0);
  if(EFI_ERROR(status)){
    Print(L"[ERROR] WaitForEvent error: %r\n",status);
    Halt();
  }

  EFI_INPUT_KEY key;
  status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] ReadKeyStroke error: %r\n",status);
    Halt();
  }

  return  status;
}


/* UefiMain */
EFI_STATUS EFIAPI UefiMain( EFI_HANDLE image_handle,
                            EFI_SYSTEM_TABLE *system_table){
  EFI_STATUS status;

  /* コンソールのクリーン */
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);


  /* ASCIIロゴの表示 */
  Print(L"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/");
  Print(L"                    / ____|      _       _   / __ \\ / ____| \n");
  Print(L"                   | (___   __ _(_)_ __ | |_| |  | | (___   \n");
  Print(L"                    \\___ \\ / _` | | '_ \\| __| |  | |\\___ \\  \n");
  Print(L"                    ____) | (_| | | | | | |_| |__| |____) | \n");
  Print(L"                   |_____/ \\__,_|_|_| |_|\\__|\\____/|_____/  \n");
  Print(L"                                  by rkarsnk, based on MikanOS\n");
  Print(L"                                      Copyleft 2021 rkarsnk.jp\n");
  Print(L"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/");

  status = WaitForPressAnyKey();  
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] WaitForPressAnyKey error: %r\n", status);
    Halt();
  }
  Stall();

  /* メモリマップの取得 */
  CHAR8 memmap_buffer[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buffer), memmap_buffer, 0, 0, 0, 0};
  status = GetMemoryMap(&memmap);
  if (EFI_ERROR(status)) {
    Print(L"failed to get memory map: %r\n", status);
    Halt();
  }

  Stall();

  /* メモリマップのファイルへの書き出し */
  EFI_FILE_PROTOCOL* root_dir;
  status = OpenRootDir(image_handle, &root_dir);
  if (EFI_ERROR(status)) {
    Print(L"failed to open root directory: %r\n", status);
    Halt();
  }
  Stall();

  EFI_FILE_PROTOCOL *memmap_file;
  status = root_dir->Open(  root_dir, 
                            &memmap_file, 
                            L"\\memmap.csv",
                            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 
                            0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\memmap.csv': %r\n", status);
    Print(L"Ignored.\n");
  } else {
    status = SaveMemoryMap(&memmap, memmap_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to save memory map: %r\n", status);
      Halt();
    }
    memmap_file->Close(memmap_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to close memory map: %r\n", status);
      Halt();
    }
  }

  Stall();

  /*フレームバッファ初期化*/
  EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
  status = OpenGOP(image_handle, &gop);
  if (EFI_ERROR(status)) {
    Print(L"failed to open GOP: %r\n", status);
    Halt();
  }
  Stall();

  Print(L"[INFO] FrameBuffer Resolution: %u * %u\n",
      gop->Mode->Info->HorizontalResolution,
      gop->Mode->Info->VerticalResolution);
  Stall();

  Print(L"[INFO] Pixel Format: %s, %u pixels/line\n",
      GetPixelFormatUnicode(gop->Mode->Info->PixelFormat),
      gop->Mode->Info->PixelsPerScanLine);
  Stall();

  Print(L"[INFO] Frame Buffer: 0x%0lx - 0x%0lx\n",
      gop->Mode->FrameBufferBase,
      gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize);
  Stall();

  Print(L"[INFO] Frame Buffer Size: %lu bytes\n", gop->Mode->FrameBufferSize);
  Stall();

  struct FrameBufferConfig config = {
    (UINT8*)gop->Mode->FrameBufferBase,
    gop->Mode->Info->PixelsPerScanLine,
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    0 
  };

  switch (gop->Mode->Info->PixelFormat) {
    case PixelRedGreenBlueReserved8BitPerColor:
      config.pixel_format = kPixelRGBResv8BitPerColor;
      break;
    case PixelBlueGreenRedReserved8BitPerColor:
      config.pixel_format = kPixelBGRResv8BitPerColor;
      break;
    default:
      Print(L"Unimplemented pixel format: %d\n", gop->Mode->Info->PixelFormat);
      Halt();
  }

  /* kernel.elfの読み込み */
  EFI_FILE_PROTOCOL* kernel_elf;
  // EFIファイルシステムをOpenし、kernel.elfを読み出す
  status = root_dir->Open(
      root_dir, &kernel_elf, L"\\kernel.elf",
      EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\kernel.elf': %r\n", status);
    Halt();
  }
  Stall();

  // file_info_size はEFI_FILE_INFOのサイズ＋ファイル名長(ヌル文字含む)
  UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * LEN_OF_KERNFILENAME;
  UINT8 file_info_buffer[file_info_size];  

  /* kernel.elfファイルの情報取得 */

  // file_info_bufferにEFI_FILE_INFO型のデータが書き込まれる
  status = kernel_elf->GetInfo(
    kernel_elf, &gEfiFileInfoGuid, 
    &file_info_size, file_info_buffer);
  if (EFI_ERROR(status)) {
    Print(L"failed to get file information: %r\n", status);
    Halt();
  }
  Stall();

  /*--------------------------------------------------------
  1.kernel_bufferに一時的なカーネルロード領域を確保
  2.カーネルELFファイルの内容をkernel_bufferに読み出す
  --------------------------------------------------------*/
  EFI_FILE_INFO* kernel_elf_info = (EFI_FILE_INFO*)file_info_buffer;
  UINTN kernel_elf_size = kernel_elf_info->FileSize;

  VOID* kernel_buffer;
  status = gBS->AllocatePool(EfiLoaderData, kernel_elf_size, &kernel_buffer);
  if(EFI_ERROR(status)) {
    Print(L"[ERROR] failed to allocate pool:%r \n", status);
    Halt();
  }

  status = kernel_elf->Read(kernel_elf, &kernel_elf_size, kernel_buffer);
  if(EFI_ERROR(status)) {
   Print(L"[ERROR] failed to read kernel file:%r \n", status);
    Halt();
  }

  /*--------------------------------------------------------
  3.kernel_bufferに配置されたkernel.elfファイルのファイルヘッダ
  　からkernelの先頭アドレスと末尾アドレスを取得する.
  --------------------------------------------------------*/
  Elf64_ElfHeader* kernel_ehdr = (Elf64_ElfHeader*)kernel_buffer;
  UINT64 kernel_head_addr, kernel_tail_addr;
  CalcLoadAddressRange(kernel_ehdr, &kernel_head_addr, &kernel_tail_addr);

  /*-------------------------------------------------------------
  4.kernelの最終配置場所のメモリ領域を必要ページ数分だけ確保する
  cf.必要ページ数の計算
  必要アドレスサイズ[Byte] = 末尾アドレス-先端アドレス
  必要ページ数 = (必要アドレスサイズ + 4095)[Byte] / 4096[Byte/Page]
                         ※ UEFIの１ページあたりのサイズは4KiB/Page.
  --------------------------------------------------------------*/
  UINTN num_pages = (kernel_tail_addr - kernel_head_addr + UEFI_PAGE_SIZE -1) / UEFI_PAGE_SIZE;
  status = gBS->AllocatePages(AllocateAddress, 
                              EfiLoaderData,
                              num_pages,
                              &kernel_head_addr);
  if (EFI_ERROR(status)) {
    Print(L"failed to allocate pages: %r\n", status);
    Halt();
  }
  Stall();

  /*--------------------------------------------------------
  5.一時領域から最終配置場所にカーネルのロードセグメントを書き出す．
  --------------------------------------------------------*/
  CopyLoadSegments(kernel_ehdr);
  Print(L"[INFO] Kernel: 0x%0lx - 0x%0lx\n", kernel_head_addr, kernel_tail_addr);
  Print(L"[INFO] Kernel size :%lu bytes)\n", kernel_elf_size);
  Stall();

  /*--------------------------------------------------------
  5.不要な一時領域を開放する．
  --------------------------------------------------------*/
  status = gBS->FreePool(kernel_buffer);
  if(EFI_ERROR(status)) {
    Print(L"[ERROR] failed to free pool:%r\n",status);
  }
  Stall();

  UINT64 entry_addr = *(UINT64*)(kernel_head_addr + ENTRY_POINT_OFFSET);
  Print(L"[INFO] Kernel entry_address: 0x%0lx \n", entry_addr);

  /* EFI BootServiceの終了 */
  status = gBS->ExitBootServices(image_handle, memmap.map_key);
  if (EFI_ERROR(status)){
    status = GetMemoryMap(&memmap);
    if (EFI_ERROR(status)){
      Print(L"failed to get memory map: %r\n", status);
      Halt();
    }
    status=gBS->ExitBootServices(image_handle, memmap.map_key);
    if(EFI_ERROR(status)){
      Print(L"Could not exit boot service: %r\n", status);
      Halt();
    }
  }

  typedef void EntryPointType(const struct FrameBufferConfig*);
  EntryPointType* entry_point = (EntryPointType*)entry_addr;
  entry_point(&config);

  Print(L"All done\n");

  Halt();
  return EFI_SUCCESS;
}
