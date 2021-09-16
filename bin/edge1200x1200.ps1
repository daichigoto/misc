#  Edgeウィンドウサイズを1200x1200にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resize.ps1 msedge .* 1200 1200"

Invoke-Expression $cmd
