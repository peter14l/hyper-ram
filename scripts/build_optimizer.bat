@echo off
setlocal
echo ======================================================================
echo   Building HyperRAM Windows 11 Utilities (Console & 24x7 Silent Daemon)
echo ======================================================================

cd /d "%~dp0\.."

where cl >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo [*] Found MSVC compiler in PATH. Compiling...
    cl /std:c++20 /O2 /EHsc /W3 src\win_optimizer.cpp /Fe:HyperRAM_Optimizer.exe psapi.lib user32.lib
    cl /std:c++20 /O2 /EHsc /W3 src\win_silent_daemon.cpp /Fe:HyperRAM_SilentDaemon.exe /link /SUBSYSTEM:WINDOWS psapi.lib user32.lib
    goto done
)

echo [*] Looking for CMake...
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target HyperRAM_Optimizer HyperRAM_SilentDaemon
copy build\Release\HyperRAM_Optimizer.exe . >nul 2>nul
copy build\Release\HyperRAM_SilentDaemon.exe . >nul 2>nul

:done
if exist HyperRAM_SilentDaemon.exe (
    echo.
    echo [SUCCESS] HyperRAM_SilentDaemon.exe and HyperRAM_Optimizer.exe built successfully!
    echo Location: %CD%\HyperRAM_SilentDaemon.exe
) else (
    echo [!] Build completed in build\Release\
)

echo.
pause
