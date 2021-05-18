#pragma once

#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#include "Include/frame_buffer_config.h"

EFI_STATUS OpenGOP(EFI_HANDLE image_handle, EFI_GRAPHICS_OUTPUT_PROTOCOL **gop);
const CHAR16 *GetPixelFormatUnicode(EFI_GRAPHICS_PIXEL_FORMAT fmt);
EFI_STATUS SaveGopModeList(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop,
                           EFI_FILE_PROTOCOL *file);
