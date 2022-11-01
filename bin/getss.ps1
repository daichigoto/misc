#!/usr/bin/env pwsh

#========================================================================
# スクリーンショットを取得する
#========================================================================

#========================================================================
# 引数を処理
#   -URI uri		スクリーンショットを取得するリソースのURI
#   -Width width	スクリーンショットの幅
#   -Height height	スクリーンショットの高さ
#   -HeightFull		スクリーンショットの高さをページ全体に設定
#   -OutputFilePath path スクリーンショットを保存するファイル
#   -Agent		Webサーバへ送るUser-Agent文字列を指定
#========================================================================
Param(
	[Parameter(Mandatory=$false)][String]$URI = "desktop:",
	[Int]$Width = 1200,
	[Int]$Height = 800,
	[Switch]$HeightFull,
	[String]$OutputFilePath = "${env:HOME}/ss.png",
	[String]$Agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"
)

#========================================================================
# スクリーンショットの取得に利用するアプリケーションほか
#========================================================================
$msedge='C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe'
$snippingtool='SnippingTool.exe'
$curl='C:\Windows\System32\curl.exe'

#========================================================================
# スクリーンショットを保存するファイルのパスを絶対パスへ変換
#========================================================================
if	(-not (Split-Path -IsAbsolute $OutputFilePath))
{
	$OutputFilePath = [System.IO.Path]::GetFullPath($OutputFilePath)
	Write-Warning 							`
		"スクリーンショット保存先として $OutputFilePath を使用。"
}

#========================================================================
# どの方法でスクリーンショットを取得するか判断
#========================================================================
switch	-Wildcard ($URI)
{
	'http*'
	{
		#========================================================
		# Microsoft Edgeを使って取得
		#========================================================
		$method='msedge'
		break
	}

	'desktop:*'
	{
		#========================================================
		# Snipping Toolを使って取得
		#========================================================
		$method='snippingtool'
		break
	}

	default
	{
		$method='none'
	}
}

#========================================================================
# スクリーンショットを撮るリソース種類の特定
#========================================================================
$contenttype='none'
switch	-Wildcard ($URI)
{
	#================================================================
	# コンテンツの種類を特定
	#================================================================
	'http*.html'
	{
		$contenttype='text/html'
		break
	}
	'http*.htm'
	{
		$contenttype='text/html'
		break
	}
	'http*.shtml'
	{
		$contenttype='text/html'
		break
	}
	'http*/'
	{
		$contenttype='text/html'
		break
	}
	'http*.txt'
	{
		$contenttype='text/plain'
		break
	}
	'http*.csv'
	{
		$contenttype='text/csv'
		break
	}
	'http*csv=1'
	{
		$contenttype='text/csv'
		break
	}
	'http*.pdf'
	{
		$contenttype='application/pdf'
		break
	}
	'http*.zip'
	{
		$contenttype='application/zip'
		break
	}
	default
	{
		$contenttype=(	& $curl		--location		`
						-A $Agent		`
						-Ss -I			`
						$URI			|
				Select-String	"^Content-Type:"	)
		break
	}
}

#========================================================================
# スクリーンショットが取れないWebリソースの場合、代わりに
# http://example.com/ のスクリーンショットを取っておく
#========================================================================
switch	-Wildcard ($contenttype)
{
	#================================================================
	# コンテンツの種類を特定
	#================================================================
	'*text/html*'
	{
		break
	}
	'*text/plain*'
	{
		break
	}
	'*/*'
	{
		$URI='http://example.com/'
		break
	}
}

#========================================================================
# ページ全体を取得する指定になっている場合にページ高さを取得
#========================================================================
if	($HeightFull)
{
	#================================================================
	# Seleniumモジュールがない場合にはインストールする
	#================================================================
	if (-Not (Get-InstalledModule -Name Selenium 2> $Null)) {
		"Seleniumモジュールをインストールします。"
		Install-Module -Name Selenium -AllowPrerelease -Force
	}

	#================================================================
	# Firefoxをヘッドレスモードで起動してページ長を計測する
	#================================================================
	Start-SeDriver	-Browser Firefox				`
			-StartURL $URI					`
			-PrivateBrowsing				`
			-Size "$Width,800"				`
			-State Headless					`
			-UserAgent $Agent				> $Null
	$Height	= Invoke-SeJavascript 					`
			-Script 'return document.body.clientHeight'
	Stop-SeDriver

	# ※ Seleniumモジュール経由でEdgeを起動する場合、ヘッドレスモードが
	# 使用できないため代わりにFirefoxを使っている。Edgeがヘッドレス
	# モードで起動できるのであればEdgeへ実装を置き換えることも可能。
}

#========================================================================
# スクリーンショットを取得
#========================================================================
switch	($method)
{
	#================================================================
	# Microsoft Edgeを使って取得
	#================================================================
	'msedge'
	{
		$o1='--headless'
		$o2='--screenshot="' + $OutputFilePath + '"'
		$o3="--window-size=$Width,$Height"
		$o4='--user-agent="$Agent"'
		$o5='--hide-scrollbars'
		$o6='--virtual-time-budget=10000'
		# ページ読み込み後にコンテンツをロードするタイプの
		# Webページはしばらく待つ必要があるため、そのために
		# --virtual-time-budget= で待機時間を指定している。

		Start-Process	-FilePath $msedge			`
				-ArgumentList $o1,$o2,$o3,$o4,$o5,$o6,$URI `
				-Wait
		break
	}

	#================================================================
	# Snipping Toolを使って取得
	#================================================================
	'snippingtool'
	{
		Start-Process	-FilePath $snippingtool	
		# ※ -Waitは指定しても機能しない
		break
	}
}
