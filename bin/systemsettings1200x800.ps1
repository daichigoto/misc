#  設定ウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + "\window_resizer.ps1 ApplicationFrameHost 設定 1200 800"

Invoke-Expression $cmd
