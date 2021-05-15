#pragma once

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>

#include "Include/memory_map.h"

EFI_STATUS GetMemoryMap(struct MemoryMap *map);
EFI_STATUS SaveMemoryMap(struct MemoryMap *map,
                         EFI_FILE_PROTOCOL *file);
