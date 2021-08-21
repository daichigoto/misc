#=========================================================================
# BSD MakefileとWindows GNU Makefileを共存させるためのラッパースクリプト
#=========================================================================

# MSYS2
$make = "C:\msys64\usr\bin\make.exe"

# make.exeをインストール
if (! (Test-Path -PathType Leaf $make)) {
	pacman -S --noconfirm make
}

# Makefile.winがある場合にはこれを指定してmake.exeを実行
if (Test-Path -PathType Leaf Makefile.win) {
	$cmd = $make + " -f Makefile.win " + $Args
	Invoke-Expression $cmd
}
else {
	$cmd = $make + " " + $Args
	Invoke-Expression $cmd
}


## Cygwin note
#$make = "C:\cygwin64\bin\make.exe"
#
## make.exeをインストール
#if (! (Test-Path -PathType Leaf $make)) {
#	echo "Install make"
#}
#
## Makefile.winがある場合にはこれを指定してmake.exeを実行
#if (Test-Path -PathType Leaf Makefile.win) {
#	$cmd = $make + " -f Makefile.win " + $Args
#	Invoke-Expression $cmd
#}
#else {
#	$cmd = $make + " " + $Args
#	Invoke-Expression $cmd
#}

# MSYS2 note
# make.exeをインストール
# if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\pacman.exe)) {
# 	winget install MSYS2
# 	C:\msys64\usr\bin\pacman.exe -Syu --noconfirm
# }
# if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\make.exe)) {
# 	pacman -S --noconfirm make
# }
