@echo off
title HyperRAM 24x7 Real-Time Background Memory Daemon
echo ======================================================================
echo   Starting HyperRAM 24x7 Real-Time Zero-Swap Engine
echo   Target: Windows 11 on 4GB / 8GB Low-RAM PCs
echo ======================================================================
echo.

cd /d "%~dp0\.."

if exist "%CD%\HyperRAM_Optimizer.exe" (
    "%CD%\HyperRAM_Optimizer.exe" %*
) else if exist "%CD%\build\Release\HyperRAM_Optimizer.exe" (
    "%CD%\build\Release\HyperRAM_Optimizer.exe" %*
) else (
    echo [!] Executable not found. Please compile first by running:
    echo     scripts\build_optimizer.bat
    echo.
    pause
)
