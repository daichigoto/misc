#!/usr/bin/env pwsh

#====================================================================
# ウィンドウリサイズスクリプト
#====================================================================

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",	# プロセス名
	[String]$WindowTitle=".*",	# ウィンドウタイトル(正規表現)
	[Int32]$Width="0",		# ウィンドウ幅
	[Int32]$Height="0",		# ウィンドウ高さ
	[Int32]$WidthRatio="0",		# ウィンドウ幅(スクリーン幅に対する割合)
	[Int32]$HeightRatio="0",	# ウィンドウ高さ(スクリーン高に対する割合)
	[Switch]$WindowProcessList	# ウィンドウプロセス一覧を表示
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

	// スクリーンサイズを取得する関数
	[DllImport("user32.dll")]
	public static extern int GetSystemMetrics(int nIndex); 
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

	# 取得したスクリーン幅からウィンドウの幅を計算
	if ($WidthRatio -ne 0) {
		# スクリーン幅を取得
		$screenWidth = [WinAPI]::GetSystemMetrics(0);
		# Widthの値を算出
		$Width = $screenWidth * $WidthRatio / 100
	}
	# 取得した座標データからウィンドウの現在の幅を計算
	elseif ($Width -eq 0) {
		$Width = $rc.Right - $rc.Left;
	}

	# 取得したスクリーン幅からウィンドウの高さを計算
	if ($HeightRatio -ne 0) {
		# スクリーン高を取得
		$screenHeight = [WinAPI]::GetSystemMetrics(1);
		# Heightの値を算出
		$Height = $screenHeight * $HeightRatio / 100
	}
	# 取得した座標データからウィンドウの現在の幅を計算
	elseif ($Height -eq 0) {
		$Height = $rc.Bottom - $rc.Top;
	}

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
