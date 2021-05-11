# 5章
## osbook_day05a
### フォント描画関数でやっていることに関するメモ
WriteAscii関数では8x16のピクセルのうち指定したピクセルだけ色を塗るために，以下の方法をとっている．

```
for(int dx = 0; dx < 8 ; ++dx){
    if ((kFontA[dy] << dx) & 0x80u){
        writer.Write(x + dx, y + dy, color);
    }
}
```

`0x80u`は2進数表記に変換すると，`0b10000000`となる.kFontA[dy]の値が`0xc1(0b11000001)`であった場合,\
`(kFontA[dy] << dx) & 0x80u`の起算結果は，下のようになる．

```
dx=0: 0b11000001 & 0b10000000 = 0b10000000 (0x80u !=0)　塗る
dx=1: 0b10000010 & 0b10000000 = 0b10000000 (0x80u !=0)　塗る
dx=2: 0b00000100 & 0b10000000 = 0b00000000 (0x00u ==0)　塗らない
```

先頭の8bit目が1の場合は計算結果が0以外になり，8bit目が0の場合は0となる．C++では，0がfalse，0以外がtrueとなるため，計算結果が０ではない要素に該当するピクセルを指定した色になり変えることになる．

## osbook_day05b
### Makefileの変更点に関するメモ
分割コンパイルに伴い，`.%.d: %.cpp`以下の変更が加えられている．
```
kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS)  -o kernel.elf $(OBJS)

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< 

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)
```
何やってるのか調べるために，`make -n`でドライラン．

```
$ make -n
clang++ -I../../tools/x86_64-elf/include/c++/v1 \
        -I../../tools/x86_64-elf/include \
        -I../../tools/x86_64-elf/include/freetype2 \
        -I../../tools/edk2/MdePkg/Include \
        -I../../tools/edk2/MdePkg/Include/X64 \
        -I../../src/bootloader/Include \
        -I../../src/kernel/Include \
        -nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -DEFIAPI='__attribute__((ms_abi))' -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 \
        -c main.cpp 
clang++ -I../../tools/x86_64-elf/include/c++/v1 \
        -I../../tools/x86_64-elf/include \
        -I../../tools/x86_64-elf/include/freetype2 \
        -I../../tools/edk2/MdePkg/Include \
        -I../../tools/edk2/MdePkg/Include/X64 \
        -I../../src/bootloader/Include \
        -I../../src/kernel/Include \
        -nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -DEFIAPI='__attribute__((ms_abi))' -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 \
        -c graphics.cpp 
clang++ -I../../tools/x86_64-elf/include/c++/v1 \
        -I../../tools/x86_64-elf/include \
        -I../../tools/x86_64-elf/include/freetype2 \
        -I../../tools/edk2/MdePkg/Include \
        -I../../tools/edk2/MdePkg/Include/X64 \
        -I../../src/bootloader/Include \
        -I../../src/kernel/Include \
        -nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -DEFIAPI='__attribute__((ms_abi))' -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 \
        -c font.cpp 

ld.lld -L../../tools/x86_64-elf/lib --entry KernelMain -z norelro -z separate-code --image-base 0x100000 \
--static -o kernel.elf main.o graphics.o font.o
```

この結果だけ見ると，追加部分は通ってない．