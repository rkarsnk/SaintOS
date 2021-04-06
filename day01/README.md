# 1日目
## 1. バイナリエディタでHello,Worldを作成
### 1.1. バイナリエディタで謎の数値列を入力する
以下の内容でファイルを作成する（写経）。
- 入力内容: [BOOTX64_EFI.txt](.asm/BOOTX64_EFI.txt)
### 1.2. 作成したバイナリファイルを保存する
- 保存先ファイル名:BOOTX64.EFI

## 2. C言語でHello,Worldを作成
### 2.1. ただの写経
- [hello.c](./c/hello.c)
### 2.2. コンパイル & リンク
```
$ clang -target x86_64-pc-win32-coff \
       -mno-red-zone -fno-stack-protector -fshort-wchar -Wall -c hello.c 
$ lld-link /subsystem:efi_application /entry:EfiMain /out:hello.efi hello.o
$ cp hello.efi BOOTX64.efi
```
ここでは、PEバイナリを生成している[^1]。

[^1] : いずれはelfバイナリで実験したい。

## 3. 動作確認
### 3.1. OVMF(Open Virtual Machine Firmware)の準備
```
sudo apt install ovmf
cp /usr/share/OVMF ~/Workspace/
```
`/usr/share/OVMF/`にはQEMUをUEFIモードで起動するために必要なファイルが保存されている。\
利用するファイルは`OVMF_CODE.fd`と`OVMF_VAR.fd`の２つ。\
`OVMF_CODE.fd`はUEFIのコード領域を提供し、`OVMF_VAR.fd`はNVRAM領域を提供する。


OVMFについては[Ubuntu Weekly Report 「第441回　QEMU/KVMでUEFIファームウェアを使う」(技術評論社)](https://gihyo.jp/admin/serial/01/ubuntu-recipe/0441)が参考になる。

### 3.2. ディスクイメージの作成
```
qemu-img create -f raw disk.img 200M
```
### 3.3. FAT32形式でフォーマット
```
mkfs.fat -n 'MIKANOS' -s 2 -f 2 -R 32 -F 32 disk.img
```
### 3.4. バイナリをディスクイメージに配置
```
mkdir mnt
sudo mount -o loop ./disk.img ./mnt
sudo mkdir -p ./mnt/EFI/BOOT
sudo cp BOOTX64.EFI ./mnt/EFI/BOOT/BOOTX64.EFI
sudo umount ./mnt
```
### 3.5. QEMU起動
```
qemu-system-x86_64 \
-drive if=pflash,file=~/Workspace/OVMF/OVMF_CODE.fd \
-drive if=pflash,file=~/Workspace/OVMF/OVMF_VAR.fd \
-hda disk.img
```

