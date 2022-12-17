#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

using namespace std;

// RAII wrapper for HANDLEs that automatically closes the handle when it goes out of scope
struct unique_handle
{
    HANDLE handle;
    unique_handle(HANDLE h = INVALID_HANDLE_VALUE) : handle(h) {}
    ~unique_handle() { if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle); }
    operator HANDLE() const { return handle; }
};

// RAII wrapper for HMODULEs that automatically frees the module when it goes out of scope
struct unique_module
{
    HMODULE module;
    unique_module(HMODULE h = nullptr) : module(h) {}
    ~unique_module() { if (module) FreeLibrary(module); }
    operator HMODULE() const { return module; }
};

// Gets the ID of a process with the specified name
DWORD get_process_id(wstring_view process_name)
{
    unique_handle thSnapShot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));
    if (thSnapShot == INVALID_HANDLE_VALUE)
    {
        cout << "Failed to create snapshot of processes: " << GetLastError() << endl;
        return 0;
    }

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);

    if (!Process32First(thSnapShot, &procEntry))
    {
        cout << "Failed to retrieve information about the first process in the snapshot: " << GetLastError() << endl;
        return 0;
    }

    do
    {
        if (wcscmp(process_name.data(), procEntry.szExeFile) == 0)
        {
            return procEntry.th32ProcessID;
        }
    } while (Process32Next(thSnapShot, &procEntry));

    return 0;
}

int main()
{
    // Convert ASCII process name to wide string
    wchar_t process_name[MAX_PATH] = {};
    if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, "process.exe", -1, process_name, MAX_PATH))
    {
        cout << "Failed to convert process name to wide string: " << GetLastError() << endl;
        return 1;
    }

    // Get process ID
    DWORD pid = get_process_id(process_name);
    if (!pid)
    {
        wcout << "Failed to find process " << process_name << "! Make sure it is running." << endl;
        return 1;
    }

    // Open handle to process
    unique_handle process(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));
    if (!process)
    {
        cout << "Failed to open handle to process: " << GetLastError() << endl;
        return 1;
    }
