// Copyright (c) 2026 HyperRAM Project. All Rights Reserved.
// Native Windows 24x7 Silent Background Daemon (Headless / Subsystem:Windows)
// Zero-Swap Anti-Freeze Memory Compactor for 4GB / 8GB PCs
#include "win_mem_utils.hpp"
#include <chrono>
#include <thread>
#include <atomic>

namespace {
    std::atomic<bool> g_running{true};
    const DWORD THRESHOLD_LOAD_PCT = 78;    // Auto-compact when memory load >= 78%
    const double MIN_AVAIL_GB      = 0.85;  // Auto-compact when free RAM < 850 MB
}

using namespace hyper_ram::win;

// Pure Windows GUI Subsystem Entry Point (Zero Console Window, 100% Invisible)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;

    // Prevent multiple instances running simultaneously
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "Global\\HyperRAM_Silent_Daemon_Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (hMutex) CloseHandle(hMutex);
        return 0; // Already running in background
    }

    // Lower process priority slightly so it never starves foreground user apps
    SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

    // Initial compaction on startup
    DWORD fg = GetForegroundPID();
    PerformWorkingSetCompaction(fg);

    auto last_compact_time = std::chrono::steady_clock::now();

    // 24x7 Silent Monitoring Loop (Consumes <0.02% CPU)
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        SystemRAMInfo cur = GetRAMStatus();
        auto now = std::chrono::steady_clock::now();
        double sec_since_last = std::chrono::duration<double>(now - last_compact_time).count();

        bool needs_compact = (cur.memory_load_pct >= THRESHOLD_LOAD_PCT) || 
                             (cur.avail_phys_gb < MIN_AVAIL_GB) ||
                             (sec_since_last > 180.0); // Periodic sweep every 3 minutes

        if (needs_compact && sec_since_last >= 5.0) { // 5-second cooldown
            DWORD active_pid = GetForegroundPID();
            PerformWorkingSetCompaction(active_pid);
            last_compact_time = std::chrono::steady_clock::now();
        }
    }

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }

    return 0;
}
