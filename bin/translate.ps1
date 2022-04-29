#!/usr/bin/env pwsh

#=========================================================================
# 翻訳サービスを使ったテキストの翻訳コマンド
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
$cmd = "C:\msys64\usr\bin\pip"
if (! (Test-Path -PathType Leaf $cmd)) {
	pacman -S --noconfirm python-pip
}

$cmd = "C:\msys64\usr\bin\deepl"
if (! (Test-Path -PathType Leaf $cmd)) {
	sh -c 'pip install deepl-translate'
}

#=========================================================================
# 指定されたファイルまたはパイプラインから受けとるテキストデータを翻訳
# サービスを使って翻訳
#=========================================================================

# 翻訳対象のデータを一時ファイルに保存
$_temp = New-TemporaryFile
if (0 -eq $Args.Length) {
	$Input | Out-File $_temp
}
else {
	Get-Content $Args[0] | Out-File $_temp
}

# 翻訳サービスで翻訳
$msysfilepath=$_temp.FullName.Replace('\','/').Replace('C:','/c')
sh -c "deepl English Japanese --file $msysfilepath"

# 一時ファイルを削除
Remove-Item $_temp
