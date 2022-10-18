#!/usr/bin/env pwsh

#========================================================================
# Google Chrome WebDriverを終了する
#========================================================================

#========================================================================
# WebDriverプロセスを終了
#========================================================================
if	(Get-Process -Name chromedriver 2> $Null) 
{
	'Google Chrome WebDriverを終了します。'
	Get-Process -Name chromedriver 2> $Null

	# Google Chrome WebDriverを終了
	Stop-SeDriver 2> $Null

	# まだ動作しているほかのGoogle Chrome WebDriverを終了
	if	(Get-Process -Name chromedriver 2> $Null) 
	{
		Get-Process -Name chromedriver 2> $Null | Stop-Process
	}

	'Google Chrome WebDriverを終了しました。'
}
else
{
	'Google Chrome WebDriverは動作していません。'
}
