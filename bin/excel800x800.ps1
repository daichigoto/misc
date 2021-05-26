#
#  Excelウィンドウサイズを800x800にするスクリプト
#

#========================================================================
# Application name and geometry parameters
#========================================================================
$name = "EXCEL"		# application name
$x = 0  		# x coordinate
$y = 0			# y coordinate
$widht = 800 + 2	# width
$height = 800 + 2	# heigth

#========================================================================
# Win32 API MoveWindows() function
#========================================================================
Add-Type @"
    using System;
    using System.Runtime.InteropServices;

    public class Win32Api {
        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
    }
"@

#========================================================================
# Windows resizing
#========================================================================
Get-Process -Name $name | ? { $_.MainWindowTitle -ne "" } | % {
    [Win32Api]::MoveWindow($_.MainWindowHandle, $x, $y, $widht, $height, $true)
} | Out-Null
