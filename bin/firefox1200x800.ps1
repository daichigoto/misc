#!/usr/bin/env pwsh

#========================================================================
#  Firefoxコンテンツサイズを1200x800にするスクリプト
#========================================================================

$cmd = ${PSScriptRoot} + "\window_resize.ps1 firefox .* 1212 919"

Invoke-Expression $cmd
