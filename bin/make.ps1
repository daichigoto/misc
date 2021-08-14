# BSD MakefileとWindows GNU Makefileを共存させるためのラッパースクリプト
#
if (Test-Path -PathType Leaf Makefile.win) {
	make.exe -f Makefile.win $Args
}
else {
	make.exe $Args
}
