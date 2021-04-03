# 『ゼロからのOS自作入門』学習記録
## 概要
『ゼロからのOS自作入門』(著者 @uchan_nos) の学習記録を管理するレポジトリ

## 記録
- [1日目](./day01)
- [2日目](./day02)

## EDK2のダウンロード
```
git clone --recursive git@github.com:tianocore/edk2.git
```
`--recursive`をつけないとsubmoduleがダウンロードできず、BaseToolsのビルドに失敗する