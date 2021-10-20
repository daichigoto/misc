#!/usr/bin/env pwsh

#========================================================================
# Windows Terminalウィンドウサイズを1200x600にするスクリプト
#========================================================================

$cmd = ${PSScriptRoot} + '\window_resize.ps1 WindowsTerminal .* 1200 600'

Invoke-Expression $cmd
