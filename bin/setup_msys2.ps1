#!/usr/bin/env pwsh

#=========================================================================
# MSYS2 実行環境セットアップスクリプト
#=========================================================================

#=========================================================================
# プラットフォームセットアップ
#=========================================================================
# MSYS2をインストール
if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\pacman.exe)) {
	winget install --id msys2.msys2
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

$cmd = "C:\msys64\usr\bin\man.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm man-db
}

$cmd = "C:\msys64\usr\bin\zip.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm zip
}

$cmd = "C:\msys64\usr\bin\unzip.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm unzip
}

$cmd = "C:\msys64\usr\bin\bc.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm bc
}

$cmd = "C:\msys64\usr\bin\neofetch"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm neofetch
}

$cmd = "C:\msys64\mingw64\bin\bat.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm mingw-w64-x86_64-bat
}

$cmd = "C:\msys64\mingw64\bin\gm.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm mingw-w64-x86_64-graphicsmagick
}

$cmd = "C:\msys64\mingw64\bin\drill.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm mingw-w64-x86_64-ldns
}

$cmd = "C:\msys64\mingw64\bin\jq.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm mingw-w64-x86_64-jq
}

$cmd = "C:\msys64\usr\bin\pip"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm python-pip
}

$cmd = "C:\msys64\usr\bin\deepl"
if (! (Test-Path -PathType Leaf $cmd)) {
	sh -c 'pip install deepl-translate'
}
