#!/usr/bin/env pwsh

#=========================================================================
# MSYS2 実行環境セットアップスクリプト
#=========================================================================

#=========================================================================
# プラットフォームセットアップ
#=========================================================================
# MSYS2をインストール
if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\pacman.exe)) {
	winget install MSYS2
	C:\msys64\usr\bin\pacman.exe -Syu --noconfirm
}

#=========================================================================
# コマンドセットをセットアップ
#=========================================================================
$cmd = "C:\msys64\usr\bin\make.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm make
}

$cmd = "C:\msys64\usr\bin\vim.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm vim
}

$cmd = "C:\msys64\usr\bin\git.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm git
}

$cmd = "C:\msys64\usr\bin\tree.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm tree
}

$cmd = "C:\msys64\mingw64\bin\gm.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm mingw-w64-x86_64-graphicsmagick
}