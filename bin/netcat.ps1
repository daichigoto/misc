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
# Webリソースの種類を取得
#========================================================================
switch	-Wildcard ($URL)
{
	'*.html' {
		$ContentType='text/html'
		break
	}
	'*.htm' {
		$ContentType='text/html'
		break
	}
	'*.shtml' {
		$ContentType='text/html'
		break
	}
	'*/' {
		$ContentType='text/html'
		break
	}
	'*.txt' {
		$ContentType='text/plain'
		break
	}
	'*.csv' {
		$ContentType='text/csv'
		break
	}
	'*csv=1' {
		$ContentType='text/csv'
		break
	}
	'*.pdf' {
		$ContentType='application/pdf'
		break
	}
	'*.zip' {
		$ContentType='application/zip'
		break
	}
	default {
		$ContentType=(&	$curl		--location 		`
						-A $Agent		`
						-Ss -I 			`
						$URL			|
				Select-String	"^Content-Type:")
		break
	}
}

#========================================================================
# どの方法でWebリソースを取得するかを判断
#========================================================================
switch	-Wildcard ($ContentType)
{
	#================================================================
	# HTMLコンテンツ: JavaScriptでコンテンツを表示するタイプのページ
	# にも対応するため、なるべくWebブラウザのヘッドレスモードを使用
	#================================================================
	'*text/html*' {
		#========================================================
		# Microsoft Edgeを使って取得
		#========================================================
		$method='msedge'
		break
	}
	#================================================================
	# それ以外のコンテンツは curl を使って取得
	#================================================================
	default {
		#========================================================
		# curlを使って取得
		#========================================================
		$method='curl'
		break
	}
}

#========================================================================
# Webリソースを取得
#========================================================================
switch	($method)
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

		$tmpf=New-TemporaryFile
		Start-Process	-FilePath $msedge			`
				-RedirectStandardOutput $tmpf		`
				-ArgumentList $o1,$o2,$o3,$o4,$URL	`
				-Wait
		Get-Content	$tmpf
		Remove-Item	$tmpf
		break
	}

	#================================================================
	# curl を使って取得
	#================================================================
	'curl'
	{
		& $curl		--location 				`
				-A $Agent				`
				-get $URL
		break
	}
}
