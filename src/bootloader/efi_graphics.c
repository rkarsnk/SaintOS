#include "efi_graphics.h"

/* OpenGOP GOP:Graphics Output Protocol */
EFI_STATUS OpenGOP(EFI_HANDLE image_handle,
                   EFI_GRAPHICS_OUTPUT_PROTOCOL **gop)
{
  UINTN num_gop_handles = 0;
  EFI_HANDLE *gop_handles = NULL;
  gBS->LocateHandleBuffer(
      ByProtocol,
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      &num_gop_handles,
      &gop_handles);

  gBS->OpenProtocol(
      gop_handles[0],
      &gEfiGraphicsOutputProtocolGuid,
      (VOID **)gop,
      image_handle,
      NULL,
      EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

  FreePool(gop_handles);

  return EFI_SUCCESS;
}

/* GetPixelFormatUnicode */
const CHAR16 *GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT fmt)
{
  switch (fmt)
  {
  case PixelRedGreenBlueReserved8BitPerColor:
    return L"PixelRedGreenBlueReserved8BitPerColor";
  case PixelBlueGreenRedReserved8BitPerColor:
    return L"PixelBlueGreenRedReserved8BitPerColor";
  case PixelBitMask:
    return L"PixelBitMask";
  case PixelBltOnly:
    return L"PixelBltOnly";
  case PixelFormatMax:
    return L"PixelFormatMax";
  default:
    return L"InvalidPixelFormat";
  }
}

EFI_STATUS SaveGopModeList(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_FILE_PROTOCOL *file)
{
  CHAR8 buf[256];
  UINTN len;
  EFI_STATUS status;

  UINTN InfoSize;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
  for (int mode = 0; mode < gop->Mode->MaxMode; ++mode)
  {
    status = gop->QueryMode(gop, mode, &InfoSize, &Info);
    len = AsciiSPrint(buf, sizeof(buf), "%u, %u * %u\n",
                      mode,
                      Info->HorizontalResolution,
                      Info->VerticalResolution);
    file->Write(file, &len, buf);
  }
  return EFI_SUCCESS;
}
