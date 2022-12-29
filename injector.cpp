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

void PcreateProcessNotifyRoutineEx(
	_In_ PEPROCESS Process,
	_In_ HANDLE ProcessId,
	_In_ PPS_CREATE_NOTIFY_INFO CreateInfo
)
{
	PCHAR pszProcessNameA = nullptr;
	WCHAR pszProcessNameW[MAX_PROCESS_NAME_LENGTH] = { 0 };
	size_t pcbProcessNameLength = 0;
	BOOLEAN bIsOnList = FALSE;

	pszProcessNameA = (PCHAR)PsGetProcessImageFileName(Process);
	bIsOnList = IsProcessInInjectionList(pszProcessNameA);

	if (NULL == CreateInfo || FALSE == bIsOnList)
	{
		if (TRUE == bIsOnList)
		{
			DbgPrint("Removing %s [%d] from list", pszProcessNameA, ProcessId);
			RtlStringCbLengthA(
				(STRSAFE_PCNZCH)pszProcessNameA,
				MAX_PROCESS_NAME_LENGTH,
				&pcbProcessNameLength
			);
			RtlMultiByteToUnicodeN(
				pszProcessNameW, MAX_PROCESS_NAME_LENGTH * 2, 
				NULL, 
				pszProcessNameA,
				pcbProcessNameLength
			);
			pProcessLinkedList->RemoveEntryByData(pszProcessNameW, (ULONG)ProcessId);
		}
		return;
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
		MessageBox(0, "Invalid Process Name!", "Injectora", MB_ICONEXCLAMATION);
		isReady = false;
		return 1;
	}

	if (strlen(filePath.getCharPointer()) < 5)
	{
		printf("Select a DLL first!\n");
		isReady = false;
		return 2;
	}

	File file(filePath);
	if (!file.exists())
	{
		MessageBox(0, "File selected to be injected does not exist!", "Injectora", MB_ICONERROR);
		isReady = false;
		return 2;
	}

	if (!Setup())
	{
		isReady = false;
		return 3;
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

	CloseHandle(processHandle);
	
	isReady = false;

	if (closeOnInject)
		PostQuitMessage(0);
	
	return 0;
}
