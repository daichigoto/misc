#!/usr/bin/env pwsh

#====================================================================
# ウィンドウ移動スクリプト
#====================================================================

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",	# プロセス名
	[String]$WindowTitle=".*",	# ウィンドウタイトル(正規表現)
	[Int32]$X="0",			# ウィンドウ左上X座標。負値は右下逆X座標
	[Int32]$Y="0",			# ウィンドウ左上Y座標。負値は右下逆Y座標
	[Double]$XRatio="0",		# ウィンドウ左上X座標。負値は右下逆X座標
					# (スクリーン幅を1とし、0～1の実数で指定)
	[Double]$YRatio="0",		# ウィンドウ左上Y座標。負値は右下逆Y座標
					# (スクリーン高を1とし、0～1の実数で指定)
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
# Win32 APIのインポート
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

#====================================================================
# ウィンドウを移動する関数 Move-Window
#====================================================================
function Move-Window {
	param (
		$wh  # ウィンドウハンドラ
	)

	# スクリーン幅を取得
	$screenWidth = [WinAPI]::GetSystemMetrics(0);

	# スクリーン高さを取得
	$screenHeight = [WinAPI]::GetSystemMetrics(1);

	# ウィンドウ座標データ構造体
	$rc = New-Object RECT

	# ウィンドウの現在の座標データを取得
	[WinAPI]::GetWindowRect($wh, [ref]$rc) > $null

	# 取得した座標データからウィンドウの幅と高さを計算
	$Width = $rc.Right - $rc.Left;
	$Height = $rc.Bottom - $rc.Top;

	# マイナス指定のXおよびYを正値へ変換
	if ($X -lt 0) {
		$X = $screenWidth + $X - $Width
	}
	if ($Y -lt 0) {
		$Y = $screenHeight + $Y - $Height
	}

	# 割合指定をXおよびYの正値へ変換
	if ($XRatio -gt 0) {
		$X = $screenWidth * $XRatio
	}
	elseif ($XRatio -lt 0) {
		$X = $screenWidth + ($screenWidth * $XRatio) - $Width
	}
	if ($YRatio -gt 0) {
		$Y = $screenHeight * $YRatio 
	}
	elseif ($YRatio -lt 0) {
		$Y = $screenHeight + ($screenHeight * $YRatio) - $Height
	}

	# ウィンドウのサイズはそのままに、左上の場所を変更
	[WinAPI]::MoveWindow($wh, $X, $Y, $Width, $Height, $true) > $null
}

#====================================================================
# 対象となるウィンドウを選択し、サイズを変更
#====================================================================
Get-Process -Name $processName |
	? { $_.MainWindowHandle -ne 0 } |
	? { $_.MainWindowTitle -match "$windowTitle" } |
	% {
 		# ウィンドウを移動
		Move-Window($_.MainWindowHandle);
}
