#include <Windows.h>
#include <iostream>
#include <string>
#include <memory>
 

namespace initialise(const String& commandLine) override {
    Desktop::getInstance().setOrientationsEnabled(Desktop::allOrientations);
    mainWindow = new MainAppWindow();
}

void shutdown() override {
    mainWindow = nullptr; 
}

void anotherInstanceStarted(const String& commandLine) override { 
    // Display a warning message when more than one instance of the application is run
    AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Injectora", "Only one instance of Injectora should be run at a time!", "OK");
}


DWORD get_process_id(const wchar_t* process_name)
{
    DWORD pid = 0;

    // Create a snapshot of all processes in the system
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to create process snapshot: " << GetLastError() << std::endl;
        return pid;
    }

    // Make sure to close the snapshot when we're done
    std::unique_ptr<void, decltype(&CloseHandle)> snapshot_handle(snapshot, &CloseHandle);

    // Iterate through all processes
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);
    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            // Check if the process name matches the one we're looking for
            if (wcscmp(entry.szExeFile, process_name) == 0)
            {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    return pid;
}

DWORD get_process_id(const wchar_t* process_name) {
    // Code to implement get_process_id function
    // ...
    return 0;
}

DWORD get_process_id(const wchar_t* process_name)
{
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 process_entry = { sizeof(process_entry) };
        if (Process32First(snapshot, &process_entry))
        {
            do
            {
                if (wcscmp(process_entry.szExeFile, process_name) == 0)
                {
                    pid = process_entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &process_entry));
        }
        CloseHandle(snapshot);
    }
    return pid;
}

int main()
{
    // Convert ASCII process name to wide string
    std::string process_name_str = "process.exe";
    std::wstring process_name_wstr(process_name_str.begin(), process_name_str.end());
    const wchar_t* process_name = process_name_wstr.c_str();

    // Get process ID
    DWORD pid = get_process_id(process_name);
    if (pid == 0)
    {
        std::wcerr << L"Failed to find process " << process_name << L"! Make sure it is running." << std::endl;
        return 1;
    }

    // Open handle to process
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (process == nullptr)
    {
        std::wcerr << L"Failed to open handle to process " << process_name << L": " 
                   << GetLastError() << L". Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Make sure to close the handle when we're done
    std::unique_ptr<void, decltype(&CloseHandle)> process_handle(process, &CloseHandle);
    std::wcout << L"Successfully opened handle to process " << process_name 
               << L" with ID " << pid << std::endl;
    return 0;
}


void PcreateProcessNotifyRoutineEx(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
    // Check if the creation info is null
    if (NULL == CreateInfo)
    {
        return;
    }

    PCHAR pszProcessNameA = nullptr;
    WCHAR pszProcessNameW[MAX_PROCESS_NAME_LENGTH] = { 0 };
    size_t pcbProcessNameLength = 0;

    // Get the process image file name
    pszProcessNameA = (PCHAR)PsGetProcessImageFileName(Process);

    // Check if the process is in the injection list
    if (IsProcessInInjectionList(pszProcessNameA))
    {
        // Print a message indicating the process is being removed from the list
        DbgPrint("Removing %s [%d] from list", pszProcessNameA, ProcessId);
        // Get the length of the ASCII process name
        RtlStringCbLengthA(
            (STRSAFE_PCNZCH)pszProcessNameA,
            MAX_PROCESS_NAME_LENGTH,
            &pcbProcessNameLength
        );
        // Convert the process name from ASCII to Unicode
        RtlMultiByteToUnicodeN(
            pszProcessNameW, MAX_PROCESS_NAME_LENGTH * 2, 
            NULL, 
            pszProcessNameA,
            pcbProcessNameLength
        );
        // Remove the process information from the linked list
        pProcessLinkedList->RemoveEntryByData(pszProcessNameW, (ULONG)ProcessId);
    }
}


 
DWORD Injector::GetProcessId()
{
	ULONG cbBuffer = 131072;
	void* pBuffer = NULL;
	NTSTATUS Status = STATUS_INFO_LENGTH_MISMATCH;
	void* hHeap = GetProcessHeap();

	DWORD processId_ = 0;

	const char* process = processName.getCharPointer();
	std::string name(process);
	if (!strstr(process, ".exe"))
		name += ".exe";

	bool check = false;
	bool found = false;
	while (!found)
	{
		pBuffer = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cbBuffer);
		if (pBuffer == NULL)
			return 0;

		Status = fnQSI(SystemProcessInformation, pBuffer, cbBuffer, &cbBuffer);
		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			check = true;
			HeapFree(hHeap, NULL, pBuffer);
			cbBuffer *= 2;
		}
		else if (!NT_SUCCESS(Status))
		{
			HeapFree(hHeap, NULL, pBuffer);
			return 0;
		}
		else
		{
			check = false;

			PSYSTEM_PROCESSES infoP = (PSYSTEM_PROCESSES)pBuffer;
			while (infoP)
			{
				char pName[256];
				memset(pName, 0, sizeof(pName));
				WideCharToMultiByte(0, 0, infoP->ProcessName.Buffer, infoP->ProcessName.Length, pName, 256, NULL, NULL);
				if (_stricmp(name.c_str(), pName) == 0)
				{
					processId_ = infoP->ProcessId;
					found = true;

					break;
				}

				if (!infoP->NextEntryDelta)
					break;
				infoP = (PSYSTEM_PROCESSES)((unsigned char*)infoP + infoP->NextEntryDelta);
			}
			if (pBuffer)
				HeapFree(hHeap, NULL, pBuffer);
		}

		if (processId_ != 0)
		{
			break;
		}
		else if (!check)
		{
			// Don't continuously search...
			break;
		}
	}

	return processId_;
}

HRESULT Injector::ManualMap(String filePath)
{
    DLL = filePath;

    if (!autoInject)
        isReady = true;

    if (!isReady)
        return 0;

    if (!CheckValidProcessExtension(processName.getCharPointer()))
    {
        MessageBox(0, "Invalid Process Name!", "Injector", MB_ICONEXCLAMATION);
        isReady = false;
        return 1;
    }

    if (filePath.isEmpty() || strlen(filePath.getCharPointer()) < 5)
    {
        printf("Error: Please specify a valid file path for the DLL to inject.\n");
        isReady = false;
        return 2;
    }

    File file(filePath);
    if (!file.exists())
    {
        printf("Error: The specified DLL file does not exist.\n");
        isReady = false;
        return 2;
    }

    if (!Setup())
    {
        printf("Error: Failed to set up injection process.\n");
        isReady = false;
        return 3;
    }

    return 0;
}


	HMODULE ret = remoteLoader.LoadLibraryByPathIntoMemoryA(filePath.toStdString().c_str(), false);
	if (!ret)
	{
		MessageBox(0, "Failed to Manual Map inject!", "Injectora", MB_ICONERROR);
		isReady = false;
		return 4;
	}

	// Beep of success
	MessageBeep(MB_OK);
	//if (!closeOnInject && !autoInject) {
	//	MessageBox(0, "Manual Map Success!", "Injectora", MB_ICONASTERISK);
	//}
	
	oldProcessIds.add(processId);

	{
	
    uint32_t address = 0x0;
    address = (addr + Read<uint32_t>(addr) + off) - (uint64_t)modEntry.modBaseAddr;	
    
    return address;
    
	}
	

void Exec::Vehicle::HornBoost() {
	if (natives::player::is_player_pressing_horn(natives::player::player_id()))
	{
		int Veh = natives::ped::get_vehicle_ped_is_in(natives::player::get_player_ped_script_index(natives::player::player_id()), false);
		natives::network::network_request_control_of_entity(Veh);
		if (natives::network::network_has_control_of_entity(Veh))
		{
			natives::vehicle::set_vehicle_forward_speed(Veh, 60);
		}
	}
}


