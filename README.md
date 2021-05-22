# SaintOS
カナやん（rkarsnk）がまったり作っているOS．
## ロゴ
```
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  / ____|      _       _   / __ \ / ____|
 | (___   __ _(_)_ __ | |_| |  | | (___  
  \___ \ / _` | | '_ \| __| |  | |\___ \ 
  ____) | (_| | | | | | |_| |__| |____) |
 |_____/ \__,_|_|_| |_|\__|\____/|_____/ 
              by rkarsnk, based on MikanOS
                 Copyleft 2021 rkarsnk.jp.
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
```

ASCIIロゴジェネレーター(https://patorjk.com/software/taag/) を使って作成しています．

## 開発環境
### 使用しているOS/Tool

|項目|名称|
|---|---|
|OS |Ubuntu 20.04 x64|
|Cコンパイラ|Clang 10.0.0 (10.0.0-4ubuntu1)|
|リンカ|LLD 10.0.0 (10.0.0-4ubuntu1)|
|エミュレーター|QEMU 4.2.1 (1:4.2-3ubuntu6.15)|
|UEFI SDK|EDK II (edk2-stable202102)|


### ディレクトリ構成
```
SaintOS
├── Makefile
├── README.md             //このファイル
├── docs
│   └── HOW_TO_DEBUG.md
├── debug                 //デバッグ用EFIディレクトリ
│   └── startup.nsh
├── log                   //QEMUのdebug.log出力先
├── hello
│   ├── asm
│   └── c
├── note                  //作業ノート管理用ディレクトリ
├── src
│   ├── bootloader
│   └── kernel
└── tools
    ├── edk2              //git submodule
    ├── mkfont            //uchan:mikanos よりコピー
    └── x86_64-elf        //uchan:mikanos-build よりコピー
```

## 準備
edk2のリポジトリの取得します．
```
make prep
```
内部処理では，EDK II(edk2-stable202102)を取得します．

## ビルド
次のコマンドでビルドします．
```
make all
```

## デモ
次のコマンドでQEMU用のディスクイメージを作成し，QEMUを起動します．
```
make run
```

## TODO
- [ ] ブートローダ内での解像度変更処理を汎用的なものにする．
- [ ] printkが25行80列のコンソールに固定しているので，解像度に合わせて可変できるようにする．
- [ ] インラインアセンブラでinb/w/l,outb/w/lを実装する．

## 参考文献
- 内田公太:"ゼロからのOS自作入門",株式会社マイナビ出版(2021).
- MikanOS, https://github.com/uchan-nos/mikanos
- EDK II, https://github.com/tianocore/edk2
- UEFI Specifications Ver2.8B, https://uefi.org/specifications/
- フルスクラッチで作る!UEFIベアメタルプログラミング, http://yuma.ohgami.jp/UEFI-Bare-Metal-Programming/index.html
