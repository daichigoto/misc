# Microsoft Storeウィンドウサイズを1200x800にするスクリプト

$cmd = ${PSScriptRoot} + '\window_resize.ps1 ApplicationFrameHost "Microsoft Store" 1200 800'

Invoke-Expression $cmd
