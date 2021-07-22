#  Excelウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 EXCEL .* 1200 800"

Invoke-Expression $cmd
