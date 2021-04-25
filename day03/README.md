# 3日目
## QEMUを使って、レジスタとメモリ内部を見てみる
### レジスタダンプ
```
(qemu) info registers
RAX=0000000000000000 RBX=0000000000000001 RCX=000000003fb7b1c0 RDX=000000000000002f
RSI=0000000000000400 RDI=000000003fea9270 RBP=000000000000002c RSP=000000003fea88a0
R8 =0000000000000000 R9 =000000003fecc30f R10=00000000000001e0 R11=0000000000000000
R12=000000003e662568 R13=000000003fea8930 R14=000000003e7727a0 R15=000000003fea88e8
RIP=000000003e661416 RFL=00000202 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
CS =0038 0000000000000000 ffffffff 00af9a00 DPL=0 CS64 [-R-]
SS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
DS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
FS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
LDT=0000 0000000000000000 0000ffff 00008200 DPL=0 LDT
TR =0000 0000000000000000 0000ffff 00008b00 DPL=0 TSS64-busy
GDT=     000000003fbee698 00000047
IDT=     000000003f306018 00000fff
CR0=80010033 CR2=0000000000000000 CR3=000000003fc01000 CR4=00000668
DR0=0000000000000000 DR1=0000000000000000 DR2=0000000000000000 DR3=0000000000000000 
DR6=00000000ffff0ff0 DR7=0000000000000400
EFER=0000000000000500
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=00000000000000000000000000000000 XMM01=00000000000000000000000000000000
XMM02=00000000000000000000000000000000 XMM03=00000000000000000000000000000000
XMM04=00000000000000000000000000000000 XMM05=00000000000000000000000000000000
XMM06=00000000000000000000000000000000 XMM07=00000000000000000000000000000000
XMM08=00000000000000000000000000000000 XMM09=00000000000000000000000000000000
XMM10=00000000000000000000000000000000 XMM11=00000000000000000000000000000000
XMM12=00000000000000000000000000000000 XMM13=00000000000000000000000000000000
XMM14=00000000000000000000000000000000 XMM15=00000000000000000000000000000000
```

### RIPレジスタが指すメモリの内容を見る
```
(qemu) x /4xb 0x03e661416
000000003e661416: 0xeb 0xfe 0x55 0x41
```

### 逆アセンブルしてみる
```
(qemu) x /2i 0x03e661416
0x3e661416:  eb fe                    jmp      0x3e661416
0x3e661418:  55                       pushq    %rbp
```

## カーネル
### main.cpp
```
extern "C" void KernelMain()
{
    while (1)
        __asm__("hlt");
}
```
hlt命令を実行し続けるだけ。hlt命令はCPUを停止させるので、省電力状態を維持できる。

### コンパイル
```
clang++ -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	-fno-exceptions -fno-rtti -std=c++17 -c main.cpp
ld.lld --entry KernelMain -z norelro --image-base 0x100000 --static \
    -o kernel.elf main.o
```
#### コンパイラオプション
|オプション||説明|
|---|---|---|
|-O2|Flags controlling how much optimization should be performed.|レベル２の最適化|
|-Wall|Enable the specified warning(-W`<warning>`) |警告をたくさん出す|
|-g|Generate source-level debug information|デバッグ情報つきでコンパイル|
|--target=x86_64-elf|Generate code for the given target `<x86_64-elf>`|x86_64向けのELF形式で出力する|
|-ffreestanding|Assert that the compilation takes place in a freestanding environment|フリースタンディング環境向け|
|-mno-red-zone|Do not use a so-called "red zone" for x86-64 code.|Red Zonn機能を無効にする|
|-fno-exceptions|Disable support for exception handling|C++の例外機能を使わない|
|-fno-rtti|Disable generation of information about every class with virtual functions for use by the C++ run-time type identification features ("dynamic_cast" and "typeid").|C++の動的型情報を使わない|
|-std=c++17|Language standard to compile for `<C++17>`|C++のバージョンをC++17とする|
|-c|Only run preprocess, compile, and assemble steps|コンパイルのみする。リンクはしない|

#### リンカオプション
|オプション||説明|
|---|---|---|
|--entry KernelMain|Use entry as the explicit symbol for beginning execution of your program, rather than the default entry point.|KernelMain()をエントリポイントとする。|
|-z norelro|If `relro`, create an ELF "PT_GNU_RELRO" segment header in the object.  This specifies a memory segment that should be made read-only after relocation, if supported. Don't create an ELF "PT_GNU_RELRO" segment if `norelro`.|リロケーション情報を読み込み専用にする機能を使わない|
|--image-base 0x100000|Use value as the base address of your program or dll.This is the lowest memory location that will be used when your program or dll is loaded.  To reduce the need to relocate and improve performance of your dlls, each should have a unique base address and not overlap any other dlls.|出力されたバイナリのベースアドレスを0x100000番地とする|
|-o kernel.elf|Use output as the name for the program produced by ld|出力ファイル名をkernel.elfとする|
|--static|Do not link against shared libraries.|静的リンクを行う|


## kernel実行
```
(qemu) info registers 
RAX=0000000000100000 RBX=000000003effef18 RCX=0000000000000000 RDX=0000000000000000
RSI=000000003feaca08 RDI=000000003feac9e0 RBP=000000003fea8840 RSP=000000003fea8840
R8 =000000003fea87b4 R9 =000000003fb7b48f R10=000000003fbcd018 R11=fffffffffffffffc
R12=000000003e66176c R13=000000003feac8c0 R14=000000003fea9100 R15=000000000000002c
RIP=0000000000101011 RFL=00000046 [---Z-P-] CPL=0 II=0 A20=1 SMM=0 HLT=1

GDT=     000000003fbee698 00000047
IDT=     000000003f306018 00000fff
CR0=80010033 CR2=0000000000000000 CR3=000000003fc01000 CR4=00000668
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
```
```
(qemu) x /2i 0x101011
0x00101011:  eb fd                    jmp      0x101010
0x00101013:  cc                       int3  
```
```
(qemu) x /7i 0x101000
0x00101000:  55                       pushq    %rbp
0x00101001:  48 89 e5                 movq     %rsp, %rbp
0x00101004:  66 2e 0f 1f 84 00 00 00  nopw     %cs:0(%rax, %rax)
0x0010100c:  00 00
0x0010100e:  66 90                    nop      
0x00101010:  f4                       hlt      
0x00101011:  eb fd                    jmp      0x101010
0x00101013:  cc                       int3
```