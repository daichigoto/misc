#  Firefoxコンテンツサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 firefox .* 1200 872"

Invoke-Expression $cmd
