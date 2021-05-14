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
+DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS)  -o kernel.elf $(OBJS)

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< 

+.%.d: %.cpp
+	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
+	$(eval OBJ = $(<:.cpp=.o))
+	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@
+
+.PHONY: depends
+depends:
+	$(MAKE) $(DEPENDS)
+
+-include $(DEPENDS)
```

この追加部分は，各`*.o`ファイルが利用するIncludeパスの情報を`.*.d`ファイルに出力する処理を行っている．

## osbook_day05e
ここでは，`fg_color_`,`cursor_row_`,`buffer_`など末尾に`_`がつく変数が宣言されている．\
末尾に`_`をつける理由はなんだろう．

private変数であることを明示するため．
