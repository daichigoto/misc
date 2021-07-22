#  メモ帳ウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 notepad * 1200 800"

Invoke-Expression $cmd
