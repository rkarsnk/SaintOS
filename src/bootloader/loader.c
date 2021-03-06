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
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Uefi.h>

#include "loader_internal.h"

#define LEN_OF_KERNFILENAME 12         // "\kernel.elf" を含む12文字
#define KERN_BASE_ADDR      0x100000;  // kernel base address
#define UEFI_PAGE_SIZE      0x1000     // UEFI Page size 4KiB
#define ENTRY_POINT_OFFSET  24         // ELF形式のENTRYPOINT Offset
#define FRAME_BUFFER_COLOR  255        // FrameBuffer Color white

void Stall() {
  gBS->Stall(500000);  // 0.5秒処理を遅らせる
}

/* CalcLoadAddressRange */
void CalcLoadAddressRange(Elf64_ElfHeader *ehdr, UINT64 *head, UINT64 *tail) {
  Elf64_ProgramHeader *phdr =
      (Elf64_ProgramHeader *)((UINT64)ehdr + ehdr->e_phoffset);
  *head = MAX_UINT64;
  *tail = 0;
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD)
      continue;
    *head = MIN(*head, phdr[i].p_vaddr);
    *tail = MAX(*tail, phdr[i].p_vaddr + phdr[i].p_memsz);
  }
}

/* CopyLoadSegments */
void CopyLoadSegments(Elf64_ElfHeader *ehdr) {
  Elf64_ProgramHeader *phdr =
      (Elf64_ProgramHeader *)((UINT64)ehdr + ehdr->e_phoffset);
  for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
    if (phdr[i].p_type != PT_LOAD)
      continue;

    UINT64 segm_in_file = (UINT64)ehdr + phdr[i].p_offset;
    CopyMem((VOID *)phdr[i].p_vaddr, (VOID *)segm_in_file, phdr[i].p_filesz);

    UINTN remain_bytes = phdr[i].p_memsz - phdr[i].p_filesz;
    SetMem((VOID *)(phdr[i].p_vaddr + phdr[i].p_filesz), remain_bytes, 0);
  }
}

/* Halt */
void Halt(void) {
  while (1)
    __asm__("hlt");
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
    PrintInfo(ERROR, L"WaitForEvent error: %r\n", status);
    Halt();
  }

  EFI_INPUT_KEY key;
  status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"ReadKeyStroke error: %r\n", status);
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
    PrintInfo(ERROR, L"failed to open GOP: %r\n", status);
    Halt();
  }

  /* 解像度をSXGAに変更する */
  int vga_mode = 0;
  for (int i = 0; i < gop->Mode->MaxMode; ++i) {
    UINTN gop_info_size;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *gop_info;
    gop->QueryMode(gop, i, &gop_info_size, &gop_info);
    if (gop_info->HorizontalResolution == RES_HORZ &&
        gop_info->VerticalResolution == RES_VERT) {
      vga_mode = i;
      break;
    }
  }

  //この実装は機種によっては利用出来ない場合がある
  status = gop->SetMode(gop, vga_mode);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to change resolution: %r\n", status);
    Halt();
  }

  /* Bootwait画面を表示 */
  Print(L"Booting SaintOS.");
  for (int i = 0; i < 5; ++i) {
    Stall();
    Print(L".");
  }

  /* コンソールのクリーン */
  gST->ConOut->ClearScreen(gST->ConOut);
  gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);

  /* ロゴを出力 */
  PrintLogo();

  /* UEFIイメージのrootディレクトリを開く */
  EFI_FILE_PROTOCOL *root_dir;
  status = OpenRootDir(image_handle, &root_dir);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to open root directory: %r\n", status);
    Halt();
  }

  PrintInfo(INFO, L"FrameBuffer Resolution: %u * %u\n",
            gop->Mode->Info->HorizontalResolution,
            gop->Mode->Info->VerticalResolution);

  /* メモリマップの取得 */
  CHAR8 memmap_buffer[4096 * 4];
  struct MemoryMap memmap = {sizeof(memmap_buffer), memmap_buffer, 0, 0, 0, 0};
  status                  = GetMemoryMap(&memmap);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to get memory map: %r\n", status);
    Halt();
  }

  /*　GOP Mode Listをファイルに保存する */
  EFI_FILE_PROTOCOL *goplist_file;
  status = root_dir->Open(
      root_dir, &goplist_file, L"\\goplist.txt",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to open file '\\goplist.txt': %r\n", status);
    PrintInfo(ERROR, L"Ignored.\n");
  } else {
    status = SaveGopModeList(gop, goplist_file);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"failed to save GOP list: %r\n", status);
      Halt();
    }
    goplist_file->Close(goplist_file);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"failed to close GOP list file: %r\n", status);
      Halt();
    }
  }

  /* メモリマップをファイルに保存する */
  EFI_FILE_PROTOCOL *memmap_file;
  status = root_dir->Open(
      root_dir, &memmap_file, L"\\memmap.csv",
      EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to open file '\\memmap.csv': %r\n", status);
    PrintInfo(ERROR, L"Ignored.\n");
  } else {
    status = SaveMemoryMap(&memmap, memmap_file);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"failed to save memory map: %r\n", status);
      Halt();
    }
    memmap_file->Close(memmap_file);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"failed to close memory map: %r\n", status);
      Halt();
    }
  }

  PrintInfo(INFO, L"Pixel Format: %s, %u pixels/line\n",
            GetPixelFormatUnicode(gop->Mode->Info->PixelFormat),
            gop->Mode->Info->PixelsPerScanLine);
  PrintInfo(INFO, L"Frame Buffer: 0x%0lx - 0x%0lx\n",
            gop->Mode->FrameBufferBase,
            gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize);
  PrintInfo(INFO, L"Frame Buffer Size: %lu bytes\n",
            gop->Mode->FrameBufferSize);

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
      PrintInfo(ERROR, L"Unimplemented pixel format: %d\n",
                gop->Mode->Info->PixelFormat);
      Halt();
  }

  /* kernel.elfの読み込み */
  EFI_FILE_PROTOCOL *kernel_elf;
  // EFIファイルシステムをOpenし、kernel.elfを読み出す
  status = root_dir->Open(root_dir, &kernel_elf, L"\\kernel.elf",
                          EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to open file '\\kernel.elf': %r\n", status);
    Halt();
  }

  // file_info_size はEFI_FILE_INFOのサイズ＋ファイル名長(ヌル文字含む)
  UINTN file_info_size =
      sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * LEN_OF_KERNFILENAME;
  UINT8 file_info_buffer[file_info_size];

  /* kernel.elfファイルの情報取得 */

  // file_info_bufferにEFI_FILE_INFO型のデータが書き込まれる
  status = kernel_elf->GetInfo(kernel_elf, &gEfiFileInfoGuid, &file_info_size,
                               file_info_buffer);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to get file information: %r\n", status);
    Halt();
  }

  /*--------------------------------------------------------
  1.kernel_bufferに一時的なカーネルロード領域を確保
  2.カーネルELFファイルの内容をkernel_bufferに読み出す
  --------------------------------------------------------*/
  EFI_FILE_INFO *kernel_elf_info = (EFI_FILE_INFO *)file_info_buffer;
  UINTN kernel_elf_size          = kernel_elf_info->FileSize;

  VOID *kernel_buffer;
  status = gBS->AllocatePool(EfiLoaderData, kernel_elf_size, &kernel_buffer);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to allocate pool:%r \n", status);
    Halt();
  }

  status = kernel_elf->Read(kernel_elf, &kernel_elf_size, kernel_buffer);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to read kernel file:%r \n", status);
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
    PrintInfo(ERROR, L"failed to allocate pages: %r\n", status);
    Halt();
  }

  /*--------------------------------------------------------
  5.一時領域から最終配置場所にカーネルのロードセグメントを書き出す．
  --------------------------------------------------------*/
  CopyLoadSegments(kernel_ehdr);
  PrintInfo(INFO, L"Kernel: 0x%0lx - 0x%0lx\n", kernel_head_addr,
            kernel_tail_addr);
  PrintInfo(INFO, L"Kernel size :%lu bytes\n", kernel_elf_size);

  /*--------------------------------------------------------
  5.不要な一時領域を開放する．
  --------------------------------------------------------*/
  status = gBS->FreePool(kernel_buffer);
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"failed to free pool:%r\n", status);
  }

  UINT64 entry_addr = *(UINT64 *)(kernel_head_addr + ENTRY_POINT_OFFSET);
  PrintInfo(INFO, L"Kernel entry_address: 0x%0lx \n", entry_addr);

  status = WaitForPressAnyKey();
  if (EFI_ERROR(status)) {
    PrintInfo(ERROR, L"WaitForPressAnyKey error: %r\n", status);
    Halt();
  }

  /* EFI BootServiceの終了 */
  status = gBS->ExitBootServices(image_handle, memmap.map_key);
  if (EFI_ERROR(status)) {
    status = GetMemoryMap(&memmap);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"failed to get memory map: %r\n", status);
      Halt();
    }
    status = gBS->ExitBootServices(image_handle, memmap.map_key);
    if (EFI_ERROR(status)) {
      PrintInfo(ERROR, L"Could not exit boot service: %r\n", status);
      Halt();
    }
  }

  typedef void EntryPointType(const struct FrameBufferConfig *,
                              const struct MemoryMap *);
  EntryPointType *entry_point = (EntryPointType *)entry_addr;
  entry_point(&config, &memmap);

  Print(L"All done\n");

  Halt();
  return EFI_SUCCESS;
}
