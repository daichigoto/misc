#!/usr/bin/env pwsh

#====================================================================
# 引数を処理
#====================================================================
Param(
	[String]$ProcessName="*",   # プロセス名
	[String]$WindowTitle=".*",  # ウィンドウタイトル(正規表現)
	[Int32]$X="0",              # ウィンドウ左上X座標
	[Int32]$Y="0",              # ウィンドウ左上Y座標
	[Int32]$Width="0",          # ウィンドウ幅
	[Int32]$Height="0",         # ウィンドウ高さ
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
# ウィンドウを配置する関数 Move-Deploy
#====================================================================
function Move-Deploy {
	param (
		$wh  # ウィンドウハンドラ
	)

	# ウィンドウ座標データ構造体
	$rc = New-Object RECT

	# ウィンドウの現在の座標データを取得
	[WinAPI]::GetWindowRect($wh, [ref]$rc) > $null

	# 幅の指定がない場合、取得した座標データからウィンドウの幅を計算
	if ($Width -eq 0) {
		$Width = $rc.Right - $rc.Left;
	}

	# 高さの指定がない場合、取得した座標データからウィンドウの高さを計算
	if ($Height -eq 0) {
		$Height = $rc.Bottom - $rc.Top;
	}

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

	# ウィンドウを指定あれた座標に指定されたサイズで配置する
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
		Move-Deploy($_.MainWindowHandle);
}
