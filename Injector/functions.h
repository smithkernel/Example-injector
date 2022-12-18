#pragma once
#include "includes.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace Functions
{
    bool DoesFileExist(const char* name)
    {
        std::ifstream file(name);
        return file.good();
    }

    DWORD GetProcessId(const char* processName)
    {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(pe32);

        if (!Process32First(hSnap, &pe32))
        {
            CloseHandle(hSnap);
            return 0;
        }

        do
        {
            if (std::strcmp(pe32.szExeFile, processName) == 0)
            {
                CloseHandle(hSnap);
                return pe32.th32ProcessID;
            }

        } while (Process32Next(hSnap, &pe32));

        CloseHandle(hSnap);
        return 0;
    }

    uintptr_t GetModuleBaseAddress(DWORD pid, const char* modName)
    {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (hSnap != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry))
            {
                do
                {
                    if (std::strcmp(modEntry.szModule, modName) == 0)
                    {
                        CloseHandle(hSnap);
                        return reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }

        return 0;
    }

    bool LoadLibraryInject(DWORD processId, const char* dll)
    {
        if (processId == 0)
        {
            return false;
        }

        char customDll[MAX_PATH];
        GetFullPathName(dll, MAX_PATH, customDll, nullptr);

        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(customDll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        if (!WriteProcessMemory(hProcess, allocatedMem, customDll, sizeof(customDll), NULL))
        {
            return false;
        }

        CreateRemoteThread(hProcess, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibrary), allocatedMem, 0, nullptr);

        if (hProcess)
        {
            CloseHandle(hProcess);
        }

        return true;
    }
}
