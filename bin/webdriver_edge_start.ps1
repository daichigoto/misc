#!/usr/bin/env pwsh

#========================================================================
# Microsoft Edge WebDriverを起動する
#========================================================================

#========================================================================
# 動作しているMicrosoft Edge WebDriverをすべて終了
#========================================================================
webdriver_edge_stop.ps1

#========================================================================
# Seleniumモジュールがない場合にはインストール
#========================================================================
if (-Not (Get-InstalledModule -Name Selenium 2> $Null)) {
	'Seleniumモジュールをインストールします。'
	Install-Module -Name Selenium -AllowPrerelease -Force
	Get-InstalledModule -Name Selenium
}

#========================================================================
# Microsoft Edge WebDriverを起動
#========================================================================
'Microsoft Edge WebDriverを起動します。'
$Size = '1200,800'

#XXX 後藤 2023-11-07
# これまでは下記処理で対応するEdge WebDriverを使っているかどうか判断する
# ことができたが、先日から異なるバージョンでも起動するようになった。
# このため、起動するものの処理ができない状況が出てきた。そのため、強制的に
# 最新のバージョンのドライバを使うように処理を変更する。状況が戻る可能性も
# あるので、コメントアウトでの対応としておく。
#
#if	(-Not (Start-SeDriver -Browser Edge -Size $Size 2> $Null 3> $Null))
#{
	#================================================================
	# Microsoft EdgeとMicrosoft Edge WebDriverのバージョンが一致して
	# いないためにドライバが動作しなかった可能性がある。
	#================================================================

	#================================================================
	# 不要なドライバプロセスを終了
	#================================================================
	webdriver_edge_stop.ps1

	#================================================================
	# Microsoft Edgeのバージョン番号
	#================================================================
	$EdgeDir='C:\Program Files (x86)\Microsoft\Edge\Application\'
	$EdgeVersion=(	Get-ChildItem -Name $EdgeDir			| 
			Where-Object { $_ -NotMatch "[a-zA-Z]+" }	|
			Select-Object -First 1				)
			# ↑ 【Select-Object -First 1の理由】
			# 更新前のバージョンと更新後のバージョンが同時に
			# 存在するタイミングがあるので、更新後のバージョン
			# のみを取得するためにSelect-Objectを実行している。

	#================================================================
	# Microsoft Edge WebDriverダウンロードURLとデプロイ先パス
	#================================================================
	$DriverURL="https://msedgedriver.azureedge.net/$EdgeVersion/edgedriver_win64.zip"

	$SeModVer=(Get-InstalledModule -Name Selenium).Version -replace "-.+$",""
	$DriverDir="$env:HOME\Documents\powershell\Modules\Selenium\$SeModVer\assemblies"
	$DriverDownloadDir="$DriverDir\_download"

	#================================================================
	# WebDriverダウンロード用の一時ディレクトリを作成
	#================================================================
	New-Item	$DriverDownloadDir -ItemType Directory -Force

	#================================================================
	# Microsoft Edgeと同じバージョンのMicrosoft Edge WebDriverを
	# ダウンロード
	#================================================================
	"Microsoft Edge WebDriver version $EdgeVersion をダウンロードします。"
	curl		-get						`
			-o	$DriverDownloadDir\edgedriver_win64.zip	`
	 		$DriverURL

	#================================================================
	# Microsoft Edge WebDriverをデプロイ
	#================================================================
	"Microsoft Edge WebDriver version $EdgeVersion をインストールします。"
	Expand-Archive	-Path $DriverDownloadDir\edgedriver_win64.zip	`
			-Destination $DriverDownloadDir			`
			-Force
			
	Copy-Item	-Path $DriverDownloadDir\msedgedriver.exe	`
			-Destination $DriverDir\msedgedriver.exe	`
			-Force

	#================================================================
	# WebDriverダウンロード用の一時ディレクトリを削除
	#================================================================
	Remove-Item	$DriverDownloadDir -Recurse -Force

	#================================================================
	# Microsoft Edge WebDriverを起動する
	#================================================================
	if	(-Not (Start-SeDriver -Browser Edge -Size $Size 2> $Null 3> $Null)) 
	{
		#========================================================
		# 原因不明の起動不能
		#========================================================

		#========================================================
		# 不要なドライバプロセスを終了
		#========================================================
		webdriver_edge_stop.ps1

		Exit
	}
#}

'Microsoft Edge WebDriverの起動処理完了。'
