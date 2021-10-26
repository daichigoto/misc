#!/usr/bin/env pwsh

#====================================================================
# ウィンドウプロセス一覧表示スクリプト
#====================================================================
Get-Process 							| 
? {$_.MainWindowHandle -ne 0 } 					| 
Format-Table -Property Id,ProcessName,MainWindowTitle
