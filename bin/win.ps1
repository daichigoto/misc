#!/usr/bin/env pwsh

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$SubCommand="",		# サブコマンド
	[String]$SubCommandArg1="",	# サブコマンド引数1
	[String]$SubCommandArg2="",	# サブコマンド引数2
	[String]$SubCommandArg3="",	# サブコマンド引数3
	[String]$SubCommandArg4="",	# サブコマンド引数4
	[String]$SubCommandArg5="",	# サブコマンド引数5
	[String]$SubCommandArg6="",	# サブコマンド引数6
	[String]$SubCommandArg7="",	# サブコマンド引数7
	[String]$SubCommandArg8="",	# サブコマンド引数8
	[String]$SubCommandArg9=""	# サブコマンド引数9
)

#====================================================================
# サブコマンドごとに処理を実行
#====================================================================
switch ($SubCommand)
{
	"msedge" {
		start Shell:AppsFolder'\'Microsoft.MicrosoftEdge_8wekyb3d8bbwe!MicrosoftEdge $SubCommandArg1
	}
	"screenshotbrowser1200x800" {
		& 'C:\Program Files\Mozilla Firefox\firefox.exe' $SubCommandArg1
		firefox1200x800.ps1
	}
	"screenshotbrowser1200x600" {
		& 'C:\Program Files\Mozilla Firefox\firefox.exe' $SubCommandArg1
		firefox1200x600.ps1
	}
	"mail" {
		& 'C:\Program Files\Mozilla Thunderbird\thunderbird.exe' -compose $SubCommandArg1
	}
	"clip" {
		if ("" -eq $SubCommandArg1) {
			clip.exe
		}
		else {
			clip.exe $SubCommandArg1
		}
	}
	default {
		Invoke-Expression ( $SubCommand + " " + $SubCommandArg1 + " " + $SubCommandArg2 + " " + $SubCommandArg3 + " " + $SubCommandArg4 + " " + $SubCommandArg5 + " " + $SubCommandArg6 + " " + $SubCommandArg7 + " " + $SubCommandArg8 + " " + $SubCommandArg9 )
	}
}
