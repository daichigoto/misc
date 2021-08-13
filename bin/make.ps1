# Unix系とWindowsでのmakeを共存させるためのラッパースクリプト
if (Test-Path -PathType Leaf Makefile.win) {
	make.exe -f Makefile.win
}
else {
	make.exe 
}
