Write-Host "======================================================================" -ForegroundColor Cyan
Write-Host "  Building HyperRAM Windows 11 Utilities (Console & 24x7 Silent Daemon)" -ForegroundColor Cyan
Write-Host "======================================================================" -ForegroundColor Cyan

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Split-Path -Parent $scriptDir
Set-Location $rootDir

# Check if cl.exe is in path
if (Get-Command "cl.exe" -ErrorAction SilentlyContinue) {
    Write-Host "[*] Compiling with MSVC cl.exe directly..." -ForegroundColor Green
    cl.exe /std:c++20 /O2 /EHsc /W3 src/win_optimizer.cpp /Fe:HyperRAM_Optimizer.exe psapi.lib user32.lib
    cl.exe /std:c++20 /O2 /EHsc /W3 src/win_silent_daemon.cpp /Fe:HyperRAM_SilentDaemon.exe /link /SUBSYSTEM:WINDOWS psapi.lib user32.lib
} else {
    Write-Host "[*] Configuring and building via CMake..." -ForegroundColor Yellow
    cmake -B build -G "Visual Studio 17 2022" -A x64
    cmake --build build --config Release --target HyperRAM_Optimizer HyperRAM_SilentDaemon
    if (Test-Path "build\Release\HyperRAM_Optimizer.exe") {
        Copy-Item "build\Release\HyperRAM_Optimizer.exe" "HyperRAM_Optimizer.exe" -Force
    }
    if (Test-Path "build\Release\HyperRAM_SilentDaemon.exe") {
        Copy-Item "build\Release\HyperRAM_SilentDaemon.exe" "HyperRAM_SilentDaemon.exe" -Force
    }
}

if (Test-Path "HyperRAM_SilentDaemon.exe") {
    Write-Host "`n[SUCCESS] HyperRAM_SilentDaemon.exe has been built!" -ForegroundColor Green
    Write-Host "File Path: $rootDir\HyperRAM_SilentDaemon.exe" -ForegroundColor White
    Write-Host "• 100% Invisible (No black console window popup)" -ForegroundColor Yellow
    Write-Host "• Double-click it once, and it will run silently in the background 24x7!`n" -ForegroundColor Cyan
} elseif (Test-Path "build\Release\HyperRAM_SilentDaemon.exe") {
    Write-Host "`n[!] Executable located at: $rootDir\build\Release\HyperRAM_SilentDaemon.exe" -ForegroundColor Yellow
}
