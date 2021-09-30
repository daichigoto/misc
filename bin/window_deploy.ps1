#!/usr/bin/env pwsh

#====================================================================
# ウィンドウ配置スクリプト
#====================================================================

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",	# プロセス名
	[String]$WindowTitle=".*",	# ウィンドウタイトル(正規表現)
	[Int32]$X="0",			# ウィンドウ左上X座標
	[Int32]$Y="0",			# ウィンドウ左上Y座標
	[Int32]$Width="0",		# ウィンドウ幅
	[Int32]$Height="0",		# ウィンドウ高
	[Double]$XRatio="-1",		# ウィンドウ左上X座標(スクリーン幅を1とし、0～1の実数で指定)
	[Double]$YRatio="-1",		# ウィンドウ左上Y座標(スクリーン高を1とし、0～1の実数で指定)
	[Double]$WidthRatio="-1",	# ウィンドウ幅(スクリーン幅を1とし、0～1の実数で指定)
	[Double]$HeightRatio="-1",	# ウィンドウ高(スクリーン高を1とし、0～1の実数で指定)
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
# ウィンドウを配置する関数 Deploy-Window
#====================================================================
function Deploy-Window {
	param (
		$wh  # ウィンドウハンドラ
	)

	# ウィンドウ座標データ構造体
	$rc = New-Object RECT

	# ウィンドウの現在の座標データを取得
	[WinAPI]::GetWindowRect($wh, [ref]$rc) > $null

	# マイナス指定のXおよびYを正値へ変換
	if ($X -lt 0) {
		# スクリーン幅を取得
		$screenWidth = [WinAPI]::GetSystemMetrics(0);
		# Xの値を算出
		$X = $screenWidth + $X - $Width
	}
	if ($Y -lt 0) {
		# スクリーン高さを取得
		$screenHeight = [WinAPI]::GetSystemMetrics(1);
		# Yの値を算出
		$Y = $screenHeight + $Y - $Height
	}

	# 座標が0～1の割合で指定されている場合にXおよびYの正値へ変換
	if ($XRatio -ge 0) {
		# スクリーン幅を取得
		$screenWidth = [WinAPI]::GetSystemMetrics(0);
		# Xの値を算出
		$X = $screenWidth * $XRatio
	}
	if ($YRatio -ge 0) {
		# スクリーン高さを取得
		$screenHeight = [WinAPI]::GetSystemMetrics(1);
		# Yの値を算出
		$Y = $screenHeight * $YRatio 
	}

	# 幅が0～1の割合で指定されている場合にWidthの値へ変換
	if ($WidthRatio -ge 0) {
		# スクリーン幅を取得
		$screenWidth = [WinAPI]::GetSystemMetrics(0);
		# Widthの値を算出
		$Width = $screenWidth * $WidthRatio / 100
	}
	# 幅指定がないため、取得した座標データからウィンドウの現在の幅を設定
	elseif ($Width -eq 0) {
		$Width = $rc.Right - $rc.Left;
	}

	# 高が0～1の割合で指定されている場合にHeightの値へ変換
	if ($HeightRatio -ge 0) {
		# スクリーン高を取得
		$screenHeight = [WinAPI]::GetSystemMetrics(1);
		# Heightの値を算出
		$Height = $screenHeight * $HeightRatio / 100
	}
	# 高指定がないため、取得した座標データからウィンドウの現在の高を設定
	elseif ($Height -eq 0) {
		$Height = $rc.Bottom - $rc.Top;
	}

	# ウィンドウを指定された座標に指定されたサイズで配置する
	[WinAPI]::MoveWindow($wh, $X, $Y, $Width, $Height, $true) > $null
}

#====================================================================
# 対象となるウィンドウを選択し、サイズを変更
#====================================================================
Get-Process -Name $processName |
	? { $_.MainWindowHandle -ne 0 } |
	? { $_.MainWindowTitle -match "$windowTitle" } |
	% {
 		# ウィンドウを配置
		Deploy-Window($_.MainWindowHandle);
}
