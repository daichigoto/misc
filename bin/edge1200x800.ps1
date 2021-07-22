#  Edgeウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 msedge .* 1200 800"

Invoke-Expression $cmd
