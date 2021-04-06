# 2日目
## 1. EDK2を利用したHello,Worldの作成
### 1.1. EDK2の取得
```
cd ~/Workspace
git clone --recursive git@github.com:tianocore/edk2.git
```
`--recursive`オプションをつけないとsubmoduleがダウンロードできず、BaseToolsのビルドに失敗する

### 1.2. EDK2 BaseToolsのビルド
```
$ cd ~/Workspace/edk2
$ make -C BaseTools/Source/C
```
※1.1と1.2の作業はmikanos-build内のplaybookで自動化されているが、自学のために手動で実行した。

### 1.3. プログラムの作成
- EDK2アプリ
  - [MikanLoaderPkg/Main.c](./MikanLoaderPkg/Main.c)
- コンポーネント定義ファイル
  - [MikanLoaderPkg/Loader.inf](./Loader.inf)
- パッケージ宣言ファイル
  - [MikanLoaderPkg/MikanLoaderPkg.dec](./MikanLoaderPkg/MikanLoaderPkg.dec)
- パッケージ記述ファイル
  - [MikanLoaderPkg/MikanLoaderPkg.dsc](./MikanLoaderPkg/MikanLoaderPkg.dsc)

### 1.4. プログラムのビルド
```
$ cd ~/Workspace/edk2
$ ln -s ../day02/MikanLoaderPkg ./
$ source edksetup.sh
```
生成されたConf/target.txtの以下の内容を変更する

|項目|値|
|--- |---|
|ACTIVE_PLATFORM | MikanLoaderPkg/MikanLoaderPkg.dsc|
|TARGET|DEBUG|
|TARGET_ARCH|X64|
|TOOL_CHAIN_TAG|CLANG38|

EDK2付属のbuildコマンド使用してビルドする.(コマンドの本体は`edk2/BaseTools/BinWrappers/PosixLike/build`)
```
$ cd ~/Workspace/edk2
$ build
```
`edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi`に目的のファイルが生成される。


### 1.5. 動作確認
生成されたLoader.efiをQEMUで起動し、"Hello, Mikan World!"と出力されることを確認する。
```
$ cd ~/Workspace
$ run_qemu.sh edk2/Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi
```

## 2. メモリマップの取得
