#
#  Visual Studio Codeウィンドウサイズを1200x800にするスクリプト
#

#========================================================================
# Window title and geometry parameters
#========================================================================
$title = "^.*Visual Studio Code$"		# window title
$x = 0  		# x coordinate
$y = 0			# y coordinate
$widht = 1200		# width
$height = 800		# heigth

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
Get-Process | ? { $_.MainWindowTitle -match $title } | % {
    [Win32Api]::MoveWindow($_.MainWindowHandle, $x, $y, $widht, $height, $true)
} | Out-Null
