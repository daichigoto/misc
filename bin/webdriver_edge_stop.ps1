#!/usr/bin/env pwsh

#========================================================================
# Microsoft Edge WebDriverを終了する
#========================================================================

#========================================================================
# WebDriverプロセスを終了
#========================================================================
if	(Get-Process -Name msedgedriver 2> $Null) 
{
	'動作しているMicrosoft Edge WebDriverを終了します。'
	Get-Process -Name msedgedriver 2> $Null

	# Microsoft Edge WebDriverを終了
	Stop-SeDriver 2> $Null

	# まだ動作しているほかのMicrosoft Edge WebDriverを終了
	if	(Get-Process -Name msedgedriver 2> $Null) 
	{
		Get-Process -Name msedgedriver 2> $Null | Stop-Process
	}

	'動作しているMicrosoft Edge WebDriverの終了処理完了。'
}
