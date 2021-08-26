#!/usr/bin/env pwsh

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",   # プロセス名
	[String]$WindowTitle=".*",  # ウィンドウタイトル(正規表現)
	[Int32]$X="0",              # ウィンドウ幅
	[Int32]$Y="0",              # ウィンドウ高さ
	[Switch]$WindowProcessList  # ウィンドウプロセス一覧を表示
)

#====================================================================
# ウィンドウプロセスを一覧表示
#====================================================================
if ($WindowProcessList) {
	Get-Process 						| 
	? {$_.MainWindowHandle -ne 0 } 				| 
	Format-Table -Property Id,ProcessName,MainWindowTitle
	exit
}

#====================================================================
# ウィンドウを移動する関数 Move-Window
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

function Move-Window {
	param (
		$wh  # ウィンドウハンドラ
	)

	# ウィンドウ座標データ構造体
	$rc = New-Object RECT

	# ウィンドウの現在の座標データを取得
	[WinAPI]::GetWindowRect($wh, [ref]$rc) > $null

	# 取得した座標データからウィンドウの幅と高さを計算
	$width = $rc.Right - $rc.Left;
	$height = $rc.Bottom - $rc.Top;

	# ウィンドウのサイズはそのままに、左上の場所を変更
	[WinAPI]::MoveWindow($wh, $X, $Y, $width, $height, $true) > $null
}

#====================================================================
# 対象となるウィンドウを選択し、サイズを変更
#====================================================================
Get-Process -Name $processName |
	? { $_.MainWindowHandle -ne 0 } |
	? { $_.MainWindowTitle -match "$windowTitle" } |
	% {
 		# ウィンドウサイズを変更
		Move-Window($_.MainWindowHandle);
}
