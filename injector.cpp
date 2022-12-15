#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

using namespace std;

DWORD get_process_id(string_view process_name) {
    HANDLE thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (thSnapShot == INVALID_HANDLE_VALUE) {
        cout << "Failed to create snapshot of processes!" << endl;
        return NULL;
    }

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);

    if (!Process32First(thSnapShot, &procEntry)) {
        cout << "Failed to retrieve information about the first process in the snapshot!" << endl;
        return NULL;
    }

    do {
        if (strcmp(process_name.data(), procEntry.szExeFile) == 0) {
            CloseHandle(thSnapShot);
            return procEntry.th32ProcessID;
        }
    } while (Process32Next(thSnapShot, &procEntry));

    CloseHandle(thSnapShot);
    return NULL;
}

int main() {
    uint32_t pid = get_process_id("process.exe");
    if (pid == NULL) {
        cout << "Failed to find process.exe! Make sure it is running." << endl;
        return 1;
    }

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process) {
        cout << "Failed to open handle to process.exe!" << endl;
        return 1;
    }

    char full_path[MAX_PATH];
    GetFullPathName("test.dll", MAX_PATH, full_path, nullptr);

    cout << "DLL path: " << full_path << endl;

    HMODULE hKernel32 = GetModuleHandle("kernel32.dll");
    if (!hKernel32) {
        cout << "Failed to get handle to kernel32.dll!" << endl;
        return 1;
    }

    LPTHREAD_START_ROUTINE pLoadLibrary = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(hKernel32, "LoadLibraryA"));
    if (!pLoadLibrary) {
        cout << "Failed to get address of LoadLibraryA function in kernel32.dll!" << endl;
        return 1;
    }

    LPVOID pRemoteString = VirtualAllocEx(process, nullptr, strlen(full_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteString) {
        cout << "Failed to allocate memory in process.exe for DLL path!" << endl;
        return 1;
    }

    if (!WriteProcessMemory(process, pRemoteString, full_path, strlen(full_path), nullptr)) {
        cout << "Failed to write DLL path to memory in process.exe!" << endl;
        return 1;
    }
