#include <Guid/FileInfo.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi.h>

#include "loader_internal.h"

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

#define mode_VGA 0
#define mode_SVGA 2
#define mode_XGA 6
#define mode_HD 9
#define mode_SXGA 14
#define mode_UXGA 20
#define mode_FHD 22
#define mode_WUXGA 23

void Stall() {
  // 0.5秒処理を遅らせる
  gBS->Stall(500000);
}

/* CalcLoadAddressRange */
void CalcLoadAddressRange(Elf64_ElfHeader *ehdr, UINT64 *head, UINT64 *tail) {
  Elf64_ProgramHeader *phdr =
      (Elf64_ProgramHeader *)((UINT64)ehdr + ehdr->e_phoffset);
  *head = MAX_UINT64;
  *tail = 0;
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD) continue;
    *head = MIN(*head, phdr[i].p_vaddr);
    *tail = MAX(*tail, phdr[i].p_vaddr + phdr[i].p_memsz);
  }
}

/* CopyLoadSegments */
void CopyLoadSegments(Elf64_ElfHeader *ehdr) {
  Elf64_ProgramHeader *phdr =
      (Elf64_ProgramHeader *)((UINT64)ehdr + ehdr->e_phoffset);
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD) continue;

    UINT64 segm_in_file = (UINT64)ehdr + phdr[i].p_offset;
    CopyMem((VOID *)phdr[i].p_vaddr, (VOID *)segm_in_file, phdr[i].p_filesz);

    UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz;
    SetMem((VOID *)(phdr[i].p_vaddr + phdr[i].p_filesz), remain_bytes, 0);
  }
}

/* Halt */
void Halt(void) {
  while (1) __asm__("hlt");
}

/*----------------------------------------------
 キー入力があるまで待ち、キー入力があれば返す関数
 ※https://github.com/No000/WcatOS の実装を参考
-----------------------------------------------*/
EFI_STATUS WaitForPressAnyKey() {
  EFI_STATUS status;

  Print(L"Press any key to continue:\n");

  status = gBS->WaitForEvent(1, &(gST->ConIn->WaitForKey), 0);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] WaitForEvent error: %r\n", status);
    Halt();
  }

  EFI_INPUT_KEY key;
  status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] ReadKeyStroke error: %r\n", status);
    Halt();
  }
  return EFI_SUCCESS;
}

/* UefiMain */
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,
                           EFI_SYSTEM_TABLE *system_table) {
  EFI_STATUS status;
  gST->ConOut->ClearScreen(gST->ConOut);

  /* GOPを取得する */
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
  status = OpenGOP(image_handle, &gop);
  if (EFI_ERROR(status)) {
    Print(L"failed to open GOP: %r\n", status);
    Halt();
  }

  /* 解像度をSXGAに変更する */
  //この実装は機種によっては利用出来ない場合がある
  status = gop->SetMode(gop, mode_SVGA);
  if (EFI_ERROR(status)) {
    Print(L"failed to change resolution: %r\n", status);
    Halt();
  }

  /* コンソールのクリーン */
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);

  /* ロゴを出力 */
  PrintLogo();

  status = WaitForPressAnyKey();
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] WaitForPressAnyKey error: %r\n", status);
    Halt();
  }

  Print(L"[INFO] FrameBuffer Resolution: %u * %u\n",
        gop->Mode->Info->HorizontalResolution,
        gop->Mode->Info->VerticalResolution);
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
  EFI_FILE_PROTOCOL *root_dir;
  status = OpenRootDir(image_handle, &root_dir);
  if (EFI_ERROR(status)) {
    Print(L"failed to open root directory: %r\n", status);
    Halt();
  }
  /*　GOP Mode Listをファイルに保存する */
  EFI_FILE_PROTOCOL *goplist_file;
  status = root_dir->Open(
      root_dir, &goplist_file, L"\\goplist.txt",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\goplist.txt': %r\n", status);
    Print(L"Ignored.\n");
  } else {
    status = SaveGopModeList(gop, goplist_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to save GOP list: %r\n", status);
      Halt();
    }
    goplist_file->Close(goplist_file);
    if (EFI_ERROR(status)) {
      Print(L"failed to close GOP list file: %r\n", status);
      Halt();
    }
  }

  /* メモリマップをファイルに保存する */
  EFI_FILE_PROTOCOL *memmap_file;
  status = root_dir->Open(
      root_dir, &memmap_file, L"\\memmap.csv",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
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

  Print(L"[INFO] Pixel Format: %s, %u pixels/line\n",
        GetPixelFormatUnicode(gop->Mode->Info->PixelFormat),
        gop->Mode->Info->PixelsPerScanLine);
  Print(L"[INFO] Frame Buffer: 0x%0lx - 0x%0lx\n", gop->Mode->FrameBufferBase,
        gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize);
  Print(L"[INFO] Frame Buffer Size: %lu bytes\n", gop->Mode->FrameBufferSize);
  Stall();

  struct FrameBufferConfig config = {(UINT8 *)gop->Mode->FrameBufferBase,
                                     gop->Mode->Info->PixelsPerScanLine,
                                     gop->Mode->Info->HorizontalResolution,
                                     gop->Mode->Info->VerticalResolution, 0};

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
  EFI_FILE_PROTOCOL *kernel_elf;
  // EFIファイルシステムをOpenし、kernel.elfを読み出す
  status = root_dir->Open(root_dir, &kernel_elf, L"\\kernel.elf",
                          EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(status)) {
    Print(L"failed to open file '\\kernel.elf': %r\n", status);
    Halt();
  }
  Stall();

  // file_info_size はEFI_FILE_INFOのサイズ＋ファイル名長(ヌル文字含む)
  UINTN file_info_size =
      sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * LEN_OF_KERNFILENAME;
  UINT8 file_info_buffer[file_info_size];

  /* kernel.elfファイルの情報取得 */

  // file_info_bufferにEFI_FILE_INFO型のデータが書き込まれる
  status = kernel_elf->GetInfo(kernel_elf, &gEfiFileInfoGuid, &file_info_size,
                               file_info_buffer);
  if (EFI_ERROR(status)) {
    Print(L"failed to get file information: %r\n", status);
    Halt();
  }
  Stall();

  /*--------------------------------------------------------
  1.kernel_bufferに一時的なカーネルロード領域を確保
  2.カーネルELFファイルの内容をkernel_bufferに読み出す
  --------------------------------------------------------*/
  EFI_FILE_INFO *kernel_elf_info = (EFI_FILE_INFO *)file_info_buffer;
  UINTN kernel_elf_size = kernel_elf_info->FileSize;

  VOID *kernel_buffer;
  status = gBS->AllocatePool(EfiLoaderData, kernel_elf_size, &kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] failed to allocate pool:%r \n", status);
    Halt();
  }

  status = kernel_elf->Read(kernel_elf, &kernel_elf_size, kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] failed to read kernel file:%r \n", status);
    Halt();
  }

  /*--------------------------------------------------------
  3.kernel_bufferに配置されたkernel.elfファイルのファイルヘッダ
  　からkernelの先頭アドレスと末尾アドレスを取得する.
  --------------------------------------------------------*/
  Elf64_ElfHeader *kernel_ehdr = (Elf64_ElfHeader *)kernel_buffer;
  UINT64 kernel_head_addr, kernel_tail_addr;
  CalcLoadAddressRange(kernel_ehdr, &kernel_head_addr, &kernel_tail_addr);

  /*-------------------------------------------------------------
  4.kernelの最終配置場所のメモリ領域を必要ページ数分だけ確保する
  cf.必要ページ数の計算
  必要アドレスサイズ[Byte] = 末尾アドレス-先端アドレス
  必要ページ数 = (必要アドレスサイズ + 4095)[Byte] / 4096[Byte/Page]
                         ※ UEFIの１ページあたりのサイズは4KiB/Page.
  --------------------------------------------------------------*/
  UINTN num_pages = (kernel_tail_addr - kernel_head_addr + UEFI_PAGE_SIZE - 1) /
                    UEFI_PAGE_SIZE;
  status = gBS->AllocatePages(AllocateAddress, EfiLoaderData, num_pages,
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
  Print(L"[INFO] Kernel: 0x%0lx - 0x%0lx\n", kernel_head_addr,
        kernel_tail_addr);
  Print(L"[INFO] Kernel size :%lu bytes\n", kernel_elf_size);
  Stall();

  /*--------------------------------------------------------
  5.不要な一時領域を開放する．
  --------------------------------------------------------*/
  status = gBS->FreePool(kernel_buffer);
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] failed to free pool:%r\n", status);
  }
  Stall();

  UINT64 entry_addr = *(UINT64 *)(kernel_head_addr + ENTRY_POINT_OFFSET);
  Print(L"[INFO] Kernel entry_address: 0x%0lx \n", entry_addr);

  status = WaitForPressAnyKey();
  if (EFI_ERROR(status)) {
    Print(L"[ERROR] WaitForPressAnyKey error: %r\n", status);
    Halt();
  }

  /* EFI BootServiceの終了 */
  status = gBS->ExitBootServices(image_handle, memmap.map_key);
  if (EFI_ERROR(status)) {
    status = GetMemoryMap(&memmap);
    if (EFI_ERROR(status)) {
      Print(L"failed to get memory map: %r\n", status);
      Halt();
    }
    status = gBS->ExitBootServices(image_handle, memmap.map_key);
    if (EFI_ERROR(status)) {
      Print(L"Could not exit boot service: %r\n", status);
      Halt();
    }
  }

  typedef void EntryPointType(const struct FrameBufferConfig *);
  EntryPointType *entry_point = (EntryPointType *)entry_addr;
  entry_point(&config);

  Print(L"All done\n");

  Halt();
  return EFI_SUCCESS;
}
