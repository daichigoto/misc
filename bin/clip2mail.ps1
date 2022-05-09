#!/usr/bin/env pwsh

#========================================================================
# システムクリップボードのテキストでメールを作成
#========================================================================

#========================================================================
# Thunderbirdアプリケーションパス
#========================================================================
$mailer='C:\Program Files\Mozilla Thunderbird\thunderbird.exe'

#========================================================================
# 本文の上限文字数
#========================================================================
# これ以上の値を指定するとThuderbirdがクラッシュすることがある
$bodycharlimit = 24000

#========================================================================
# 宛先とサブジェクト
#========================================================================
$to=$env:DEFAULT_EMAIL_TO
$subject="Windowsシステムクリップボード"

#========================================================================
# システムクリップボードのテキストをThunderbirdのコンポーザに貼り付け
# できるフォーマットへ変換
#========================================================================
$body=$(Get-Clipboard | Out-String)

$body=$body -replace " ","&nbsp;"
$body=$body -replace "<","&lt;"
$body=$body -replace ">","&gt;"
$body=$body -replace "	","<pre style='display:inline'>&#009;</pre>"
$body=$body -replace ",","&#044;"

#========================================================================
# 引数で指定できる文字列長には上限がある。上限を超えている場合には、上限
# 未満まで文字数を減らして使用する。
#========================================================================
function Get-TrimmedBody {
	# 上限文字数で文字列を切り捨てる。
	$trimmed = $body.Substring(0, $bodycharlimit)

	# 文字列の末尾がこの配列の文字列の途中で切れていたと判断できる
	# 場合に、配列の文字列の分も削除する。これら文字列はすべて記載
	# されている必要があり、途中で切れている場合には削除する必要が
	# ある。
	$a = $("&nbsp;",
	       "&lt;",
	       "&gt;",
	       "<pre style='display:inline'>&#009;</pre>",
	       "&#044;")
	:EndTreatment foreach ($v in $a) {
		$len = $v.Length - 1
		for ($i=0; $i -lt $len; $i++) {
			$v = $v.Substring(0, $len - $i)
			if ($trimmed.EndsWith($v)) {
				$trimmed = $trimmed.Substring(0, $trimmed.Length - $v.Length)
				break EndTreatment
			}
		}
	}

	# 終端処理を行った文字列を返す
	$trimmed
}

if ($body.Length -gt $bodycharlimit) {
	$body = Get-TrimmedBody
	$body += "<br>"
	$body += "<br>"
	$body += "(サイズオーバーでカットしました)"
}

#========================================================================
# メールを作成
#========================================================================
& $mailer 								`
	-compose 							`
	"to='$to',subject='$subject',body='$body'"
