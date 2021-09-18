#!/usr/bin/env pwsh

#========================================================================
# BSD MakefileとWindows GNU Makefileを共存させるためのラッパースクリプト
#========================================================================

# 必要になるプラットフォームとパッケージをセットアップ
msys_setup

# WindowsではGNUmakeを使用する
$make = "C:\msys64\usr\bin\make.exe"

# Makefile.winがある場合にはこれを指定してmake.exeを実行
if (Test-Path -PathType Leaf Makefile.win) {
	$cmd = $make + " -f Makefile.win " + $Args
	Invoke-Expression $cmd
}
else {
	$cmd = $make + " " + $Args
	Invoke-Expression $cmd
}
