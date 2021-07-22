#  Edgeウィンドウサイズを1200x1200にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 msedge * 1200 1200" 

Invoke-Expression $cmd
