# Chromeウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resize.ps1 chrome .* 1200 800"

Invoke-Expression $cmd
