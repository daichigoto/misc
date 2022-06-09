#!/usr/bin/env pwsh

#========================================================================
# システムクリップボードのテキストでメールを作成
#========================================================================

#========================================================================
# 引数を処理
#   -To	a@example.com,b@example.com	宛先メールアドレス
#   -Cc a@example.com,b@example.com	Ccメールアドレス
#   -Bcc a@example.com,b@example.com	Bcccメールアドレス
#   -From bar@example.com		送信元メールアドレス
#   -Subject 'Mail Title'		メールサブジェクト
#   -Attachment file1.jpg,fil2.png	添付ファイル
#========================================================================
Param(
	[String]$To = $Env:DEFAULT_EMAIL_TO,
	[String]$Cc = "",
	[String]$Bcc = "",
	[String]$From = $Env:DEFAULT_EMAIL_FROM,
	[String]$Subject = "Windowsシステムクリップボード",
	[String]$Attachment = ""
)

mail.ps1	-To		$To				`
		-Cc		$Cc				`
		-Bcc		$Bxc				`
		-From		$From				`
		-Subject	$Subject			`
		-Attachment	$Attachment			`
		-BodyFromClip
