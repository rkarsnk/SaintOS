# 2日目

## 1. EDK2を利用したHello,Worldの作成
### 1.1. EDK2の取得
```
cd ~/Workspace
git clone --recursive git@github.com:tianocore/edk2.git
```
`--recursive`オプションをつけないとsubmoduleがダウンロードできず、BaseToolsのビルドに失敗する
