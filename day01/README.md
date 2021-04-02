# 1日目
## バイナリエディタでHello,Worldを作成
以下の内容でファイルを作成する。
- 入力内容: [BOOTX64_EFI.txt](.asm/BOOTX64_EFI.txt)
- 保存先ファイル名:BOOTX64.EFI
## QEMUで動作確認
### 準備
[mikanos-build](https://github.com/uchan-nos/mikanos-build/)のレポジトリからOVMF_CODE.fdとOVMF_VARS.fdを入手する。
```
wget https://github.com/uchan-nos/mikanos-build/blob/master/devenv/OVMF_CODE.fd
wget https://github.com/uchan-nos/mikanos-build/blob/master/devenv/OVMF_VARS.fd
```

### ディスクイメージの作成
```
qemu-img create -f raw disk.img 200M
```
### FAT32形式でフォーマット
```
mkfs.fat -n 'MIKANOS' -s 2 -f 2 -R 32 -F 32 disk.img
```
### バイナリをディスクイメージに配置
```
mkdir mnt
sudo mount -o loop ./disk.img ./mnt
sudo mkdir -p ./mnt/EFI/BOOT
sudo cp BOOTX64.EFI ./mnt/EFI/BOOT/BOOTX64.EFI
sudo umount ./mnt
```
### QEMU起動
```
qemu-system-x86_64 \
-drive if=pflash,file=./OVMF_CODE.fd \
-drive if=pflash,file=./OVMF_VAR.fd \
-hda disk.img
```
## C言語でHello,Worldを作成
### ただの写経
- [hello.c](./c/hello.c)
### コンパイル & リンク
```
$ clang -target x86_64-pc-win32-coff \
       -mno-red-zone -fno-stack-protector -fshort-wchar -Wall -c hello.c 
$ lld-link /subsystem:efi_application /entry:EfiMain /out:hello.efi hello.o
```
