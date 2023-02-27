#include <Windows.h>
#include <iostream>
#include <string>
#include <memory>
 

namespace initialise(const String& commandLine) override {
    // Enable all orientations for the application's desktop
    Desktop::getInstance().setOrientationsEnabled(Desktop::allOrientations);

    // Create the main application window
    auto mainWindow = std::make_unique<MainApplicationWindow>();

    // Set the main window to be the top-level component of the application
    mainWindow->setUsingNativeTitleBar(true);
    mainWindow->centreWithSize(getWidth(), getHeight());
    mainWindow->setVisible(true);

    // Save the main window in a class member variable for later use
    m_mainWindow = std::move(mainWindow);
}

void shutdown() override {
    // Release the main window
    m_mainWindow.reset();
}

void anotherInstanceStarted(const String& commandLine) override {
    // Display a warning message when more than one instance of the application is run
    AlertWindow::showMessageBox(AlertWindow::WarningIcon,
                                "Application Already Running",
                                "Only one instance of the application can be run at a time.",
                                "OK");
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


class LoadedDll {
public:
    explicit LoadedDll(HMODULE handle) : handle_(handle) {}
    ~LoadedDll() { if (handle_) FreeLibrary(handle_); }
    HMODULE get() const { return handle_; }
private:
    HMODULE handle_ = nullptr;
};

// Inject a DLL into a remote process and return the address of a function
// Throws a std::runtime_error if the injection fails
uint32_t InjectDll(const std::string& filePath, uint32_t processId, uint32_t addr, uint32_t off) {
    // Load the DLL into memory using manual map injection
    LoadedDll dll(remote_loader::LoadLibraryByPathIntoMemoryA(filePath.c_str(), false));
    if (!dll.get()) {
        throw std::runtime_error("Failed to inject DLL: " + filePath);
    }

    // Play a system beep to indicate successful injection
    MessageBeep(MB_OK);

    // Save the process ID for future reference
    static std::vector<uint32_t> oldProcessIds;
    oldProcessIds.push_back(processId);

    // Calculate the address of the desired function in the loaded DLL
    uint32_t functionAddress = (addr + Read<uint32_t>(addr) + off) - reinterpret_cast<uint32_t>(dll.get());
    return functionAddress;
}
	
void Exec::Vehicle::HornBoost()
{
    const int playerId = natives::player::player_id();

    if (!natives::player::is_player_pressing_horn(playerId)) {
        return;
    }

    const int pedId = natives::player::get_player_ped_script_index(playerId);
    const int vehicleId = natives::ped::get_vehicle_ped_is_in(pedId, false);

    if (vehicleId == 0 || !natives::entity::does_entity_exist(vehicleId)) {
        return;
    }

    const bool hasControlOfVehicle = natives::network::network_request_control_of_entity(vehicleId);
    if (!hasControlOfVehicle) {
        return;
    }

    const float kBoostSpeed = 60.0f;
    const float currentSpeed = natives::entity::get_entity_speed(vehicleId);
    const float maxSpeed = natives::vehicle::get_vehicle_model_max_speed(natives::entity::get_entity_model(vehicleId));
    const float newSpeed = std::min(currentSpeed + kBoostSpeed, maxSpeed);

    natives::vehicle::set_vehicle_forward_speed(vehicleId, newSpeed);
}

