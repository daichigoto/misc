#=========================================================================
# BSD MakefileとWindows GNU Makefileを共存させるためのラッパースクリプト
#=========================================================================

#=========================================================================
# WindowsではGNUmakeを使用する
#=========================================================================
# MSYS2 GNUmake
$make = "C:\msys64\usr\bin\make.exe"

#=========================================================================
# プラットフォームおよびコマンドセットをセットアップ
#=========================================================================
# MSYS2をインストール
if (! (Test-Path -PathType Leaf C:\msys64\usr\bin\pacman.exe)) {
	winget install MSYS2
	C:\msys64\usr\bin\pacman.exe -Syu --noconfirm
}
# make.exeをインストール
if (! (Test-Path -PathType Leaf $make)) {
	pacman -S --noconfirm make
}
# そのほか使用頻度の高いコマンドをインストール
$vim = "C:\msys64\usr\bin\vim.exe"
if (! (Test-Path -PathType Leaf $vim)) {
	pacman -S --noconfirm vim
}
$git = "C:\msys64\usr\bin\git.exe"
if (! (Test-Path -PathType Leaf $git)) {
	pacman -S --noconfirm git
}
$tree = "C:\msys64\usr\bin\tree.exe"
if (! (Test-Path -PathType Leaf $tree)) {
	pacman -S --noconfirm tree
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
