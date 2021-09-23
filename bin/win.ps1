#!/usr/bin/env pwsh

#====================================================================
# Windowsコマンドラッパースクリプト
#====================================================================

#====================================================================
# サブコマンド抽出、引数を文字列へ変換ほか
#====================================================================
$argstr=$Args -join ' '
$subcmd=$Args[0]
$subcmdarg1=$Args[1]
$subcmdarg2=$Args[2]
$subcmdargs=$Args[1..100] -join ' '

#====================================================================
# サブコマンドごとに処理を実行
#====================================================================
switch ($subcmd)
{
	"msedge" {
		$cmd='C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe'
		& $cmd $subcmdarg1
	}
	"screenshotbrowser1200x800" {
		$cmd='C:\Program Files\Mozilla Firefox\firefox.exe'
		& $cmd $subcmdarg1
		firefox1200x800.ps1
	}
	"screenshotbrowser1200x600" {
		$cmd='C:\Program Files\Mozilla Firefox\firefox.exe'
		& $cmd $subcmdarg1
		firefox1200x600.ps1
	}
	"mail" {
		$cmd='C:\Program Files\Mozilla Thunderbird\thunderbird.exe'
		& $cmd -compose $subcmdarg1
	}
	"clip" {
		if ($null -eq $subcmdarg1) {
			$Input | clip.exe
		}
		else {
			Get-Content $subcmdarg1 | clip.exe
		}
	}
	default {
		Invoke-Expression $argstr
	}
}
