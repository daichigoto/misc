#!/usr/bin/env pwsh

#========================================================================
# Windows Terminalウィンドウサイズを1200x800にするスクリプト
#========================================================================

$cmd = ${PSScriptRoot} + '\window_resize.ps1 WindowsTerminal .* 1200 800'

Invoke-Expression $cmd
