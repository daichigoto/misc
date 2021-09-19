#!/usr/bin/env pwsh

#=========================================================================
# GNU date / BSD date コマンドラッパー
#=========================================================================

# 必要になるプラットフォームとパッケージをセットアップ
setup_msys2

# 引数に-vがある場合はBSD dateを、そうでない場合はGNU dateを実行する
echo $Args | grep -- '-v' 2>&1 > $null
if ($?) {
	$Input | bsddate $Args
}
else {
	$Input | C:\msys64\usr\bin\date.exe $Args
}
