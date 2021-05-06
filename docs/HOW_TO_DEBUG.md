# [WIP] GDBでEFIアプリケーションデバッグする
## 参考文献
- [How-to-run-OVMF (tianocore)](https://github.com/tianocore/tianocore.github.io/wiki/How-to-run-OVMF)
- [How to debug OVMF with QEMU using GDB (tianocore)](https://github.com/tianocore/tianocore.github.io/wiki/How-to-debug-OVMF-with-QEMU-using-GDB)
- [Debugging_UEFI_applications_with_GDB (wiki.osdev.org)](https://wiki.osdev.org/Debugging_UEFI_applications_with_GDB)
- [UEFI OVMF symbol load script for GDB (artem-nefedov/uefi-gdb)](https://github.com/artem-nefedov/uefi-gdb)
- [QEMUで実行されているGRUB2 EFIイメージをデバッグする](https://tutorialmore.com/questions-2526193.htm)
- [OVMFのデバッグ](https://retrage.github.io/2019/11/05/debugging-ovmf.html)

---
## DEBUG準備
UEFIモードで起動したQEMUで、debug.logを出力できるようにする。\
DEBUGに対応したOVMFが必要であるため、EDK2からビルドする。
```
$ cd edk2
$ make -C BaseTools/Source/C        <- buildに必要なC toolをビルド
$ source ./edksetup.sh
$ build -p OvmfPkg/OvmfPkgX64.dsc -b DEBUG -a X64 -t CLANG38
$ ls edk2/Build/OvmfX64/DEBUG_CLANG38/FV/ |grep OVMF
OVMF_CODE.fd
OVMF_VARS.fd
OVMF.fd
...
```

以下のオプションをつけてQEMUを起動する。
```
sudo qemu-system-x86_64 -m 1G \
   -drive if=pflash,format=raw,readonly,file=edk2/Build/OvmfX64/DEBUG_CLANG38/FV/OVMF_CODE.fd \
   -drive if=pflash,format=raw,file=edk2/Build/OvmfX64/DEBUG_CLANG38/FV/OVMF_VARS.fd \
   -drive if=ide,index=0,media=disk,format=raw,file=disk.img \
   -device nec-usb-xhci,id=xhci -device usb-mouse -device usb-kbd \
   -debugcon file:debug.log -global isa-debugcon.iobase=0x402 \
   -monitor stdio
```
上のコマンドでは、みかん本のQEMU起動オプションに対して、以下の変更を加えています。
- OVMFファイルの差し替え
- `-debugcon file:debug.log -global isa-debugcon.iobase=0x402` の追加

## debug.logを取得する
QEMUを起動すると以下のようなdebug.logが出力される
```
[debug.log]
SecCoreStartupWithStack(0xFFFCC000, 0x820000)
... (略) ...
FSOpen: Open '\EFI\BOOT\BOOTX64.EFI' Success
[Bds] Expand PciRoot(0x0)/Pci(0x1,0x1)/Ata(Primary,Master,0x0) -> PciRoot(0x0)/Pci(0x1,0x1)/Ata(Primary,Master,0x0)/\EFI\BOOT\BOOTX64.EFI
[Security] 3rd party image[0] can be loaded after EndOfDxe: PciRoot(0x0)/Pci(0x1,0x1)/Ata(Primary,Master,0x0)/\EFI\BOOT\BOOTX64.EFI.
InstallProtocolInterface: 5B1B31A1-9562-11D2-8E3F-00A0C969723B 3EB3B840
Loading driver at 0x0003E46E000 EntryPoint=0x0003E46F776 Loader.efi
... (略) ...
```
```
Loading driver at 0x0003E46E000 EntryPoint=0x0003E46F776 Loader.efi
``` 
以上の記述から、Loader.efiは `0x0003E46E000` にロードされていることがわかる。

StOSLoaderPkg/Main.cに以下のコード追記し、`loaded_image->ImageBase`の値も確認する。
```
EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;

gBS->OpenProtocol(
   image_handle,
   &gEfiLoadedImageProtocolGuid,
   (VOID **)&loaded_image,
   image_handle,
   NULL,
   EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

Print(L"UEFI Image Base :0x%0lx\n",loaded_image->ImageBase);
```
`0x0003E46F000`となっており、0x1000の差がある

![qemu_day04a-01](https://user-images.githubusercontent.com/26945036/116802382-27977180-ab4d-11eb-820d-3962d10035f9.png)


## PEバイナリの情報を確認
次にgithubから`retrage/peinfo`を取得し、Loader.efiの情報を確認する。
- [peinfo: Portable Executable Header Viewer](https://github.com/retrage/peinfo)

ちなみにpeinfoはPEバイナリ向けのreadelfのようなもの。

```
$ ./peinfo/peinfo edk2/Build/StOSLoaderX64/DEBUG_CLANG38/X64/Loader.efi
Machine: 0x8664
... (略) ...
Name: .text
VirtualSize: 0x00002280
VirtualAddress: 0x00000240
SizeOfRawData: 0x00002280
PointerToRawData: 0x00000240
... (略) ...
```

## Loader.efiの.text領域の内容を確認.
```
Loader.efi:     ファイル形式 pei-x86-64

セクション .text の逆アセンブル:
0000000000000240 <.text>:
     240:       48 83 ec 28             sub    $0x28,%rsp
     244:       48 8b 57 08             mov    0x8(%rdi),%rdx
     248:       48 85 d2                test   %rdx,%rdx
     24b:       74 2b                   je     0x278
```
QEMUのdebug.logでは、Loader.efiに `0x0003E46E000` ロードされていると出力されていたので、peinfoのVirtualAddress `0x240`を足した場所(`0x0003E46E240`) 

```
(qemu) x /4xi 0x0003E46E240
0x3e46e240:  48 83 ec 28              subq     $0x28, %rsp
0x3e46e244:  48 8b 57 08              movq     8(%rdi), %rdx
0x3e46e248:  48 85 d2                 testq    %rdx, %rdx
0x3e46e24b:  74 2b                    je       0x3e46e278
```
