#  Edgeウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resize.ps1 msedge .* 1200 800"

Invoke-Expression $cmd
