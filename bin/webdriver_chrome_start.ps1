#!/usr/bin/env pwsh

#========================================================================
# Google Chrome WebDriverを起動する
#========================================================================

#========================================================================
# 動作しているGoogle Chrome WebDriverをすべて終了
#========================================================================
webdriver_chrome_stop.ps1

#========================================================================
# Seleniumモジュールがない場合にはインストール
#========================================================================
if (-Not (Get-InstalledModule -Name Selenium 2> $Null)) {
	'Seleniumモジュールをインストールします。'
	Install-Module -Name Selenium -AllowPrerelease -Force
	Get-InstalledModule -Name Selenium
}

#========================================================================
# Google Chrome WebDriverを起動
#========================================================================
'Google Chrome WebDriverを起動します。'
$Size = '1200,800'
if	(-Not (Start-SeDriver -Browser Chrome -Size $Size 2> $Null 3> $Null))
{
	#================================================================
	# Google ChromeとGoogle Chrome WebDriverのバージョンが一致して
	# いないためにドライバが動作しなかった可能性がある。
	#================================================================

	#================================================================
	# 不要なドライバプロセスを終了
	#================================================================
	webdriver_chrome_stop.ps1

	#================================================================
	# Google Chromeのバージョン番号
	#================================================================
	$ChromeDir='C:\Program Files\Google\Chrome\Application\'
	$ChromeVersion=(Get-ChildItem -Name $ChromeDir			| 
			Where-Object { $_ -NotMatch "[a-zA-Z]+" }	)
	$ChromeMajorVersion=$ChromeVersion.Split('.')[0]

	#================================================================
	# Google Chrome WebDriverダウンロードURLとデプロイ先パス
	#================================================================
	$DriverVersion=(curl --get https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$ChromeMajorVersion 2> $Null)
	$DriverZIP='chromedriver_win32.zip'
	$DriverURL='https://chromedriver.storage.googleapis.com/' + $DriverVersion + '/' + $DriverZIP

	$SeModVer=(Get-InstalledModule -Name Selenium).Version -replace "-.+$",""

	# 2023-11-17 後藤
	# 環境変数USERNAMEが差し替えられると、環境変数HOMEも自動的に変更
	# される。これではWebDriverのパスまで変わってしまうため、変更を
	# 受けない環境変数HOMEDRIVEおよび環境変数HOMEPATHを使って表記し
	# ている。
	$DriverDir="$env:HOMEDRIVE$env:HOMEPATH\Documents\powershell\Modules\Selenium\$SeModVer\assemblies"
	$DriverDownloadDir="$DriverDir\_download"

	#================================================================
	# WebDriverダウンロード用の一時ディレクトリを作成
	#================================================================
	New-Item	$DriverDownloadDir -ItemType Directory -Force

	#================================================================
	# Google Chromeと同じバージョンのGoogle Chrome WebDriverを
	# ダウンロード
	#================================================================
	"Google Chrome WebDriver version $ChromeVersion をダウンロードします。"
	curl		-get						`
			-o	$DriverDownloadDir\$DriverZIP		`
	 		$DriverURL

	#================================================================
	# Google Chrome WebDriverをデプロイ
	#================================================================
	"Google Chrome WebDriver version $ChromeVersion をインストールします。"
	$DriverEXE='chromedriver.exe'
	Expand-Archive	-Path $DriverDownloadDir\$DriverZIP		`
			-Destination $DriverDownloadDir			`
			-Force
			
	Copy-Item	-Path $DriverDownloadDir\$DriverEXE		`
			-Destination $DriverDir\$DriverEXE		`
			-Force

	#================================================================
	# WebDriverダウンロード用の一時ディレクトリを削除
	#================================================================
	Remove-Item	$DriverDownloadDir -Recurse -Force

	#================================================================
	# Google Chrome WebDriverを起動する
	#================================================================
	if	(-Not (Start-SeDriver -Browser Chrome -Size $Size 2> $Null 3> $Null)) 
	{
		#========================================================
		# 原因不明の起動不能
		#========================================================

		#========================================================
		# 不要なドライバプロセスを終了
		#========================================================
		webdriver_chrome_stop.ps1

		Exit
	}
}
'Google Chrome WebDriverの起動処理完了。'
