#  メモ帳ウィンドウサイズを1200x1200にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 notepad * 1200 1200"

Invoke-Expression $cmd
