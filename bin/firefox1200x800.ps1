#  Firefoxコンテンツサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resize.ps1 firefox .* 1200 872"

Invoke-Expression $cmd
