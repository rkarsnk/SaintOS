[WIP] GDBでEFIアプリケーションデバッグする準備のメモ
※あとで編集します

mikanos-build付属のOVMFファイルではQEMUのdebug.logが出力されないため、
新規にdebug出力に対応したOVMFファイルを作成する。作成方法は以下のとおり。

```
$ cd edk2
$ source ./edksetup.sh
$ build -p OvmfPkg/OvmfPkgX64.dsc -b DEBUG -a X64 -t CLANG38
$ ls edk2/Build/OvmfX64/DEBUG_CLANG38/FV/ |grep OVMF
OVMF_CODE.fd
OVMF_VARS.fd
OVMF.fd  
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
...
```
以上の記述から、Loader.efiは `0x0003E45E000` にロードされていることがわかる。

念の為EFIアプリケーションからも確認。`0x0003E46E000`となっており、0x10000の差がある
![qemu_day04a-01](https://user-images.githubusercontent.com/26945036/116802382-27977180-ab4d-11eb-820d-3962d10035f9.png)





次にgithubから`retrage/peinfo`を取得し、Loader.efiの情報を確認します。
ちなみにpeinfoはPEバイナリ向けのreadelfのようなもの。

```
$ git clone git@github.com:retrage/peinfo.git
$ make -C peinfo
$ ./peinfo/peinfo edk2/Build/StOSLoaderX64/DEBUG_CLANG38/X64/Loader.efi
Machine: 0x8664
... (略) ...
Name: .text
VirtualSize: 0x00002280
VirtualAddress: 0x00000240
SizeOfRawData: 0x00002280
PointerToRawData: 0x00000240
... (略) ...

Name: .data
VirtualSize: 0x000001c0
VirtualAddress: 0x000024c0
SizeOfRawData: 0x000001c0
PointerToRawData: 0x000024c0
... (略) ...
Name: .reloc
VirtualSize: 0x00000040
VirtualAddress: 0x00002680
SizeOfRawData: 0x00000040
PointerToRawData: 0x00002680
```
