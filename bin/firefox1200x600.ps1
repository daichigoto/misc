#  Firefoxコンテンツサイズを1200x600にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resize.ps1 firefox .* 1203 677"

Invoke-Expression $cmd
