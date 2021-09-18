#!/usr/bin/env pwsh

#========================================================================
# Visual Studio Codeウィンドウサイズを1200x800にするスクリプト
#========================================================================

$cmd = ${PSScriptRoot} + '\window_resize.ps1 * "^.*Visual Studio Code$" 1200 800'

echo $cmd
Invoke-Expression $cmd
