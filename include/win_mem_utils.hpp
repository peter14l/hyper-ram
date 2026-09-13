// Copyright (c) 2026 HyperRAM Project. All Rights Reserved.
#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstddef>

namespace hyper_ram::win {

struct SystemRAMInfo {
    DWORD memory_load_pct{0};
    double total_phys_gb{0.0};
    double avail_phys_gb{0.0};
    double used_phys_gb{0.0};
};

inline SystemRAMInfo GetRAMStatus() noexcept {
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

inline DWORD GetForegroundPID() noexcept {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    return pid;
}

inline size_t PerformWorkingSetCompaction(DWORD skip_pid = 0) noexcept {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);

    size_t processes_compacted = 0;
    DWORD my_pid = GetCurrentProcessId();

    if (Process32First(snapshot, &pe)) {
        do {
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

} // namespace hyper_ram::win
#endif
