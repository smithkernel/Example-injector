#include <Windows.h>
#include <iostream>
#include <string>
#include <memory>

// Returns the process ID of the process with the given name.
// Returns 0 if the process was not found.
DWORD get_process_id(const wchar_t* process_name)
{
    // Get snapshot of all processes
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create snapshot of processes: " << GetLastError() << std::endl;
        return 0;
    }

    // Iterate through processes
    PROCESSENTRY32W process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(snapshot, &process_entry))
    {
        do
        {
            // Check if process name matches
            if (wcscmp(process_entry.szExeFile, process_name) == 0)
            {
                // Process found
                CloseHandle(snapshot);
                return process_entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &process_entry));
    }

    // Process not found
    CloseHandle(snapshot);
    return 0;
}

int main()
{
    // Convert ASCII process name to wide string
    std::string process_name_str = "process.exe";
    std::wstring process_name_wstr(process_name_str.begin(), process_name_str.end());
    const wchar_t* process_name = process_name_wstr.c_str();

    // Get process ID
    DWORD pid = get_process_id(process_name);
    if (!pid)
    {
        std::wcerr << "Failed to find process " << process_name << "! Make sure it is running." << std::endl;
        return 1;
    }

    // Open handle to process
    std::unique_ptr<void, decltype(&CloseHandle)> process(OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid), &CloseHandle);
    if (!process)
    {
        std::cerr << "Failed to open handle to process: " << GetLastError() << std::endl;
        return 1;
    }

    return 0;
}
