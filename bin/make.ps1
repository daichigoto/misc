#=========================================================================
# BSD MakefileとWindows GNU Makefileを共存させるためのラッパースクリプト
#=========================================================================

# MSYS2 make.exeをインストール
if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\pacman.exe)) {
	winget install MSYS2
	C:\msys64\usr\bin\pacman.exe -Syu --noconfirm
}
if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\make.exe)) {
	pacman -S --noconfirm make
}

# Makefile.winがある場合にはこれを指定してmake.exeを実行
if (Test-Path -PathType Leaf Makefile.win) {
	make.exe -f Makefile.win $Args
}
else {
	make.exe $Args
}
