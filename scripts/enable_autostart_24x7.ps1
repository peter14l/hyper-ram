# Adds HyperRAM_SilentDaemon.exe to Windows Startup (Run at login)
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
$exe = "$rootDir\HyperRAM_SilentDaemon.exe"

if (-not (Test-Path $exe)) {
    $exe = "$rootDir\build\Release\HyperRAM_SilentDaemon.exe"
}

if (Test-Path $exe) {
    $regKey = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
    Set-ItemProperty -Path $regKey -Name "HyperRAM_24x7_Daemon" -Value "`"$exe`""
    Write-Host "[SUCCESS] HyperRAM has been configured to start automatically on Windows boot!" -ForegroundColor Green
    Write-Host "[i] It will run silently in the background 24x7 with zero popups." -ForegroundColor Cyan
} else {
    Write-Host "[-] Please build HyperRAM_SilentDaemon.exe first using scripts\build_optimizer.bat." -ForegroundColor Red
}
