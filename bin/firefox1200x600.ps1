#  Firefoxコンテンツサイズを1200x600にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 firefox * 1203 677"

Invoke-Expression $cmd
