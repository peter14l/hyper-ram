// Copyright (c) 2026 HyperRAM Project. All Rights Reserved.
// Native Windows 24x7 Silent Background Daemon (Headless / Subsystem:Windows)
// Zero-Swap Anti-Freeze Memory Compactor for 4GB / 8GB PCs
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <string>
#include <fstream>

namespace {
    std::atomic<bool> g_running{true};
    const DWORD THRESHOLD_LOAD_PCT = 78;    // Auto-compact when memory load >= 78%
    const double MIN_AVAIL_GB      = 0.85;  // Auto-compact when free RAM < 850 MB
}

struct SystemRAMInfo {
    DWORD memory_load_pct;
    double total_phys_gb;
    double avail_phys_gb;
    double used_phys_gb;
};

SystemRAMInfo GetRAMStatus() {
    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);

    SystemRAMInfo info;
    info.memory_load_pct = mem.dwMemoryLoad;
    info.total_phys_gb = static_cast<double>(mem.ullTotalPhys) / (1024.0 * 1024.0 * 1024.0);
    info.avail_phys_gb = static_cast<double>(mem.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
    info.used_phys_gb  = info.total_phys_gb - info.avail_phys_gb;
    return info;
}

// Get PID of current active foreground window so we don't disrupt active typing or gaming
DWORD GetForegroundPID() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

// Compacts working set across accessible background processes
size_t PerformWorkingSetCompaction(DWORD skip_pid = 0) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);

    size_t processes_compacted = 0;
    DWORD my_pid = GetCurrentProcessId();

    if (Process32First(snapshot, &pe)) {
        do {
            // Skip kernel idle, current process, and active foreground window
            if (pe.th32ProcessID <= 4 || pe.th32ProcessID == my_pid || pe.th32ProcessID == skip_pid) {
                continue;
            }

            HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
            if (hProcess) {
                if (EmptyWorkingSet(hProcess)) {
                    processes_compacted++;
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(snapshot, &pe));
    }

    CloseHandle(snapshot);
    return processes_compacted;
}

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
