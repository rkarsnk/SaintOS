#include "efi_messages.h"

EFIAPI EFI_STATUS PrintInfo(UINTN level, const CHAR16* Format, ...) {
  VA_LIST args;
  EFI_STATUS status;
  CHAR16 string[256];

  VA_START(args, Format);
  status = UnicodeVSPrint(string, sizeof(string), Format, args);
  VA_END(args);

  switch (level) {
    case INFO:
      Print(L"[");
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGREEN);
      Print(L"INFO");
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      Print(L"] ");
      Print(string);
      break;

    case ERROR:
      Print(L"[");
      gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED);
      Print(L"ERROR");
      gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE);
      Print(L"] ");
      Print(string);
      break;
  }
  return status;
}

/*
UnicodeVSPrint (
  OUT CHAR16        *StartOfBuffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *FormatString,
  IN  VA_LIST       Marker
  )
*/