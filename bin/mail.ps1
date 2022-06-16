#!/usr/bin/env pwsh

#========================================================================
# メールコンポーザ起動スクリプト
#========================================================================

#========================================================================
# 引数を処理
#   -To	a@example.com,b@example.com	宛先メールアドレス
#   -Cc a@example.com,b@example.com	Ccメールアドレス
#   -Bcc a@example.com,b@example.com	Bcccメールアドレス
#   -From bar@example.com		送信元メールアドレス
#   -Subject 'Mail Title'		メールサブジェクト
#   -Body 'Mail body'			メール本文(引数で指定)
#   -BodyFromFile file1.txt		メール本文(ファイルで指定)
#   -BodyFromClip			メール本文(クリップボードからコピー)
#   -Attachment file1.jpg,fil2.png	添付ファイル
#========================================================================
Param(
	[String]$To = $Env:DEFAULT_EMAIL_TO,
	[String]$Cc = "",
	[String]$Bcc = "",
	[String]$From = $Env:DEFAULT_EMAIL_FROM,
	[String]$Subject = "メール送付",
	[String]$Body = "",
	[String]$BodyFromFile = $null,
	[Switch]$BodyFromClip,
	[String]$Attachment = ""
)

#========================================================================
# Thunderbirdアプリケーションパス
#========================================================================
$mailer = 'C:\Program Files\Mozilla Thunderbird\thunderbird.exe'

#========================================================================
# 本文の上限文字数
#========================================================================
# これ以上の値を指定するとThuderbirdがクラッシュすることがある
$bodycharlimit = 24000

#========================================================================
# メール本文を取得するとともに、Thunderbirdのコンポーザに貼り付けできる
# フォーマットへ変換
#========================================================================
if ($BodyFromClip) {
	# システムクリップボードから本文をコピーする場合
	$body = Get-Clipboard | Out-String
}
elseif ($BodyFromFile) {
	# ファイルから本文を持ってくる場合
	$body = Get-Content -Path $BodyFromFile -Raw
}

$body = $body -replace " ","&nbsp;"
$body = $body -replace "<","&lt;"
$body = $body -replace ">","&gt;"
$body = $body -replace "	","<pre style='display:inline'>&#009;</pre>"
$body = $body -replace ",","&#044;"

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
# To、Cc、BccをThunberbirdで指定できる形へ展開
#========================================================================
$to = $to.Replace(' ',',')
$cc = $cc.Replace(' ',',')
$bcc = $bcc.Replace(' ',',')

#========================================================================
# 添付ファイルのパスを絶対パスへ変換
#========================================================================
# 「,」区切りで最初のファイルを調べ、このファイルが存在しなかった場合には
# 「 」区切りで渡されていると判断し、分割処理を行う
$AttachmentFiles = $Attachment -Split ','
if (-not (Test-Path $AttachmentFiles[0])) {
	$AttachmentFiles = $Attachment -Split ' '
}

# パスを相対パスから絶対パスへ変換する
$len = $AttachmentFiles.Length
$AttachmentPaths = ""
for ($i = 0; $i -lt $len; $i++) {
	if ($AttachmentFiles[$i]) {
		$AttachmentPaths += Convert-Path $AttachmentFiles[$i]
		if ($i -ne $len - 1) {
			$AttachmentPaths += ","
		}
	}
}

#========================================================================
# メールを作成
#========================================================================
& $mailer 								`
	-compose 							`
	"to='$To',cc='$Cc',bcc='$Bcc',from='$From',subject='$Subject',attachment='$AttachmentPaths',body='$Body'"
