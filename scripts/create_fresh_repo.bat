@echo off
title Create Fresh HyperRAM GitHub Repo
echo ========================================================================
echo     Creating Standalone GitHub Repository for HyperRAM
echo ========================================================================
echo.

powershell -ExecutionPolicy Bypass -File "%~dp0create_fresh_repo.ps1"

echo.
pause
