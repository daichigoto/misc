#!/usr/bin/env pwsh

#========================================================================
# Microsoft Edge Webドライバを終了する
#========================================================================

#========================================================================
# Webドライバプロセスを終了
#========================================================================
if	(Get-Process -Name msedgedriver 2> $Null) 
{
	'動作しているMicrosoft Edge Webドライバを終了します。'
	Get-Process -Name msedgedriver 2> $Null

	# Microsoft Edge Webドライバを終了
	Stop-SeDriver 2> $Null

	# まだ動作しているほかのMicrosoft Edge Webドライバを終了
	if	(Get-Process -Name msedgedriver 2> $Null) 
	{
		Get-Process -Name msedgedriver 2> $Null | Stop-Process
	}

	'動作しているMicrosoft Edge Webドライバの終了処理完了。'
}
