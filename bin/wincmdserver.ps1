#!/usr/bin/env pwsh

#========================================================================
# 特定のファイルに書き込まれたコマンドを実行する簡易サーバ
#========================================================================

#========================================================================
# コマンド/クリップボートテキストが書き込まれるファイル
#========================================================================
$commandFilePath = "${HOME}/.wincmdserver_cmd"
$clipbrdFilePath = "${HOME}/.wincmdserver_clip"

#========================================================================
# ファイルをチェックするインターバル時間[秒]
#========================================================================
$fileCheckInterval = 1.0

#========================================================================
# サーバのプロセスIDを書き込むファイル
#========================================================================
$pidFilePath = "${HOME}/.wincmdserver_pid"

#========================================================================
# サーバのプロセスIDをファイルへ記録
#========================================================================
Write-Output $PID > $pidFilePath

#========================================================================
# コマンド/クリップボートテキストが書き込まれるファイルを初期化
#========================================================================
Write-Output $null > $commandFilePath
Write-Output $null > $clipbrdFilePath

#========================================================================
# コマンド/クリップボートテキストが書き込まれるファイルを監視して、
# コマンド/クリップボートテキストが書き込まれた場合には、コマンドを
# 実行してファイルをクリアしたり、システムクリップボートへコピーして
# ファイルをクリアする。
#========================================================================
while ($true) {
    # コマンドファイルが存在し、かつ、中身があるときに中身を実行する
    if (Test-Path "$commandFilePath") {

        if (0 -lt (Get-ChildItem "$commandFilePath").Length) {

            # ファイルに書き込まれたコマンドを実行
            Invoke-Expression (cat "$commandFilePath") 

            # ログ出力
            $timestamp = Get-Date -format "yyyy/MM/dd HH:mm:ss"
            $timestamp + " - " + (cat "$commandFilePath")

            # ファイルの中身をクリア
            Clear-Content "$commandFilePath"
        }
    }

    # クリップボートファイルが存在し、かつ、中身があるときにシステム
    # クリップボードへコピーする
    if (Test-Path "$clipbrdFilePath") {

        if (0 -lt (Get-ChildItem "$clipbrdFilePath").Length) {

            # システムクリップボードへテキストをコピー
						$OutputEncoding = [Console]::OutputEncoding
						cat "$clipbrdFilePath" | clip
						$OutputEncoding = New-Object System.Text.UTF8Encoding

            # ログ出力
            $timestamp = Get-Date -format "yyyy/MM/dd HH:mm:ss"
            $timestamp + " - copied to system clipboard"

            # ファイルの中身をクリア
            Clear-Content "$clipbrdFilePath"
        }
    }

    # 次のチェックまで指定秒間待機
    Start-Sleep $fileCheckInterval
}
