#!/usr/bin/env pwsh

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",   # プロセス名
	[String]$WindowTitle="*",  # ウィンドウタイトル
	[Int32]$Width="800",        # ウィンドウ幅
	[Int32]$Height="600"        # ウィンドウ高さ
)

#====================================================================
# ウィンドウサイズを変更する関数 Resize-Window
#====================================================================
Add-Type @"
using System;
using System.Runtime.InteropServices;

public struct RECT
{
	public int Left;
	public int Top;
	public int Right;
	public int Bottom;
}

public class WinAPI
{
	// ウィンドウの現在の座標データを取得する関数
	[DllImport("user32.dll")]
	public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

	// ウィンドウの座標を変更する関数
	[DllImport("user32.dll")]
	public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint); 
}
"@

function Resize-Window {
	param (
		$wh  # ウィンドウハンドラ
	)

	# ウィンドウ座標データ構造体
	$rc = New-Object RECT

	# ウィンドウの現在の座標データを取得
	[WinAPI]::GetWindowRect($wh, [ref]$rc) > $null

	# 左上の場所はそのままに、ウィンドウのサイズを変更
	[WinAPI]::MoveWindow($wh, $rc.Left, $rc.Top, $width, $height, $true) > $null
}

#====================================================================
# 対象となるウィンドウを選択し、サイズを変更
#====================================================================

Get-Process -Name $processName |
	? { $_.MainWindowHandle -ne 0 } |
	? { $_.MainWindowTitle -match "$windowTitle" } |
	% {
 		# ウィンドウサイズを変更
		Resize-Window($_.MainWindowHandle);
}
