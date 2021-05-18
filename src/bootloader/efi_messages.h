#pragma once

#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#define INFO 0
#define ERROR 1

EFIAPI EFI_STATUS PrintInfo(UINTN level, const CHAR16* Format, ...);
