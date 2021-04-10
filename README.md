# 『ゼロからのOS自作入門』学習記録
## 概要
『ゼロからのOS自作入門』(著者 @uchan_nos) の学習記録を管理するレポジトリ\
書籍のサポートサイトは[こちら](https://zero.osdev.jp)。

## 事前準備
### 0. 開発環境構築
開発環境にはLinux Mint 20.1 Ulyssaを採用した。UlyssaはUbuntu 20.04をベースにしたLinux ディストリビューションの一つ。\
みかん本では、執筆時期の関係でUbuntu 18.04を採用しているが、おそらくUbuntu 20.04でもいけると思う。。。

Linux Mintのインストール手順については割愛。

### 1. 作業用のディレクトリを作成する

``` sh
$ mkdir ~/Workspace
$ git init Workspace
$ touch Wokrspace/README.md
```

### 2. MikanOS関連のリポジトリを取得する
``` sh
$ git clone git@github.com:uchan-nos/mikanos-build.git ~/osbook
$ git clone git@github.com:uchan_nos/mikanos.git ~/mikanos
```

### 3. 必要なパッケージをインストールする
`ansible`をインストール後に、`mikanos-build`レポジトリが用意している`ansible-playbook`を利用して
``` sh
$ sudo apt install ansible
$ cd osbook/devenv
$ ansible-playbook -K -i ansible_inventory ansible_provision.yml
```
playbook内の処理でEDK2のリポジトリの取得も合わせて行われているが、自学のために再度取得する。\
また、このplaybookで取得しているパッケージについては、他の環境でも実現できるように別途整理する。


## 記録
- [1日目](./day01)
- [2日目](./day02)
