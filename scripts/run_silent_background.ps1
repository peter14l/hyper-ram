# Run HyperRAM in Silent Background Mode 24x7
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
$exe = "$rootDir\HyperRAM_SilentDaemon.exe"
if (-not (Test-Path $exe)) {
    $exe = "$rootDir\build\Release\HyperRAM_SilentDaemon.exe"
}

if (Test-Path $exe) {
    Start-Process -FilePath $exe
    Write-Host "[+] HyperRAM is now running silently in the background 24x7!" -ForegroundColor Green
    Write-Host "[i] It will monitor memory every 3s and auto-prevent swap freezing." -ForegroundColor Yellow
} else {
    Write-Host "[-] Executable not found. Please compile first via scripts\build_optimizer.bat." -ForegroundColor Red
}
