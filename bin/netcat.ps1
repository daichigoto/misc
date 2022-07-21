#!/usr/bin/env pwsh

#========================================================================
# URLで指定されたリソースを取得
#========================================================================

#========================================================================
# 引数を処理
#   -URL url		WebリソースのURL
#   -Agent agent	リクエストで使うエージェントを指定
#========================================================================
Param(
	[Parameter(Mandatory=$true)][String]$URL = "",
	[String]$Agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64)"
)

#========================================================================
# Webリソース取得に利用するアプリケーション
#========================================================================
$msedge='C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe'
$curl='C:\Windows\System32\curl.exe'

#========================================================================
# どの方法でWebリソースを取得するかを判断
#========================================================================
if (Test-Path $msedge) {

	#================================================================
	# Microsoft Edgeを使って取得
	#================================================================
	$method='msedge'
}
elseif (Test-Path $curl) {

	#================================================================
	# curl を使って取得
	#================================================================
	$method='curl'
}
else {
	$method='none'
}

#========================================================================
# Webリソースを取得
#========================================================================
switch ($method)
{
	#================================================================
	# Microsoft Edgeを使って取得
	#================================================================
	'msedge'
	{
		$o1='--headless'
		$o2='--dump-dom'
		$o3='--enable-logging'
		$o4='--user-agent="$agent"'
		Start-Process	-FilePath $msedge			`
				-ArgumentList $o1,$o2,$o3,$o4,$URL	`
				-Wait
	}

	#================================================================
	# curl を使って取得
	#================================================================
	'curl'
	{
		& $curl		--location 				`
				-A $Agent				`
				-get $URL
	}
}
