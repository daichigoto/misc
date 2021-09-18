#=========================================================================
# MSYS2 実行環境をセットアップ
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
# マンドセットをセットアップ
#=========================================================================
# make.exeをインストール
$cmd = "C:\msys64\usr\bin\make.exe"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm make
}

# そのほか使用頻度の高いコマンドをインストール
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
