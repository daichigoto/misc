#!/usr/bin/env pwsh

#====================================================================
# システムクリップボードのテキストでメールを作成
#====================================================================

#====================================================================
# 宛先とサブジェクト
#====================================================================
$to=$env:DEFAULT_TO_EMAIL
$title="Windowsシステムクリップボード"

#====================================================================
# システムクリップボードのテキストをThunderbirdのコンポーザに貼り付け
# できるフォーマットへ変換
#====================================================================
$body=$(Get-Clipboard | Out-String)

$body=$body -replace " ","&nbsp;"
$body=$body -replace "<","&lt;"
$body=$body -replace ">","&gt;"

#====================================================================
# メールを作成
#====================================================================
& 'C:\Program Files\Mozilla Thunderbird\thunderbird.exe' `
        -compose `
        "to='$to',subject='$title',body='$body'"
