## [補足]スタック領域の移動
8章のスタック領域の移動について、多分理解できた。
おそらくこういうことだと思う。

スタック領域の移動は、以下のアセンブラで実装している。

``` [kernel_entry.asm]
extern kernel_main_stack
extern KernelMainNewStack


global KernelMain
KernelMain:
    mov rsp, kernel_main_stack + 1024 * 1024
    call KernelMainNewStack
.fin:
    hlt
    jmp .fin
```
```
0000000000106a10 <KernelMain>:
  106a10:       48 bc 00 80 21 00 00    movabs $0x218000,%rsp
  106a17:       00 00 00 
  106a1a:       e8 e1 d5 ff ff          callq  104000 <KernelMainNewStack>
```
私の環境では、kernel_main_stackの先頭アドレスは0x00118000、末尾アドレスは0x00218000となっている。

RSPレジスタがkernel_main_stack領域の末尾アドレスを差すようにしてから、C++で実装されたKernelMainNewStack呼び出すという上記の実装を追加することで、
kernel_main_stackで確保したメモリ領域をkernel用のスタック領域として利用できるようにしている。

RSPレジスタの値はCPUのスタックポインタが差しているアドレスなので、RSPレジスタにUEFIのスタック領域とは別のクリーン？なメモリ領域を差すアドレスを設定してあげれば、
UEFIのスタック領域を使用していた状態から脱却できる。

## ユーザ定義リテラルとは


[ユーザ定義リテラル](https://cpprefjp.github.io/lang/cpp11/user_defined_literals.html)

```
  constexpr unsigned long long operator""_KiB(unsigned long long kib) {  // NN_KiB
    return kib * 1024;
  }

  constexpr unsigned long long operator""_MiB(unsigned long long mib) {  // NN_MiB
    return mib * 1024_KiB;
  }

  constexpr unsigned long long operator""_GiB(unsigned long long gib) {  // NN_GiB
    return gib * 1024_MiB;
  }
```
上記のように宣言すると、以下のようになる。
```
4_MiB == 4 * 1024_KiB == 4 * 1024 * 1024 
```

## explicit
```
explicit FrameID(size_t id)
```