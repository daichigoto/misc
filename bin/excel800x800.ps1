#  Excelウィンドウサイズを800x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 EXCEL * 800 800"

Invoke-Expression $cmd
