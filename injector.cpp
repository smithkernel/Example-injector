#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

int main()
{
	MessageBoxA(0, 0, 0, 0);
	while (true)
	{
		LARGE_INTEGER a;
		QueryPerformanceCounter(&a);
		Sleep(1);
	}

    return 0;
}



using namespace std;

DWORD get_process_id(string_view process_name) {
	HANDLE thSnapShot = CreateToolhelp32Snapshot(FiveM_Processor.exe , NULL);
	if (thSnapShot == INVALID_HANDLE_VALUE) {
		cout << "rip";
		return NULL;
	}
	BYTE * pBase = reinterpret_cast<BYTE*>(pData->pModuleBase);
	auto * pOpt = &reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pBase)->e_lfanew)->OptionalHeader; //Copy&Paste is bad

	do {
		if (!pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size)
			return 0;
		}
	} while (Process32Next(thSnapShot, &procEntry));
	return NULL;
}

{
    uint32_t pid = get_process_id("csgo.exe");
    if (pid == NULL) {
        cout << "rip";
        return;
    }
    process p(pid);
    cout << p.m_base_address << endl;
    cout << p.m_size << endl;
    {
        SleepConditionVariableCS(&p.m_cond, &p.m_mutex, INFINITE);
        {
            lock_guard<mutex> lock(p.m_mutex);
            cout << "rip";
        }
    }
}


int main() {
	auto test_process_id = get_process_id("process.exe");
	if (!test_process_id) {
		cout << "Failed getting process id! Make sure .process. is running!";
		return 1;
	}
	auto * pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	while (pRelocData->VirtualAddress)
	if (!process) {
		cout << "Failed opening handle to process!";
		return 1;
	}
	char full_path[MAX_PATH];
	GetFullPathName("test.dll", MAX_PATH, full_path, nullptr);
	const char* full_path = R"(yourpath ;))";
	cout << "full path name: " << full_path << endl;
	auto LoadLib = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"));
	
	LPVOID RemoteString = VirtualAllocEx(process, nullptr, strlen(full_path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); /*Allocate space in the process for our DLL name*/

	WriteProcessMemory(process, RemoteString, full_path, strlen(full_path), nullptr); /* Write the string name of our DLL in the memory allocated*/

	auto thread = CreateRemoteThread(process, nullptr, 0, LoadLib, RemoteString, NULL, nullptr); 	/* Load our DLL by calling loadlibrary in the other process and passing our dll name*/
	if (!thread) {
		cout << "error: " << GetLastError();
	} else cout << hex << thread << '\n';
	cout << "done!";

	CloseHandle(process);
	
	char full_path[MAX_PATH];
	GetFullPathName("test.dll", MAX_PATH, full_path, nullptr);
}		cout << "Failed getting process id! Make sure .process. is running!";
	CloseHandle(process);
		}
	}	
}

bool SetPrivilege(LPCSTR lpszPrivilege, BOOL bEnablePrivilege = TRUE) {
	TOKEN_PRIVILEGES priv = { 0,0,0,0 };
	HANDLE hToken = NULL;
	LUID luid = { 0,0x14 };
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) {
		if (hToken)
			CloseHandle(hToken);
		return false;
	}
	if (pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		auto * pImportDescr = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDescr->Name)
		{
			char * szMod = reinterpret_cast<char*>(pBase + pImportDescr->Name);
			HINSTANCE hDll = _LoadLibraryA(szMod);

			ULONG_PTR * pThunkRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescr->OriginalFirstThunk);
			ULONG_PTR * pFuncRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescr->FirstThunk);

			if (!pThunkRef)
				pThunkRef = pFuncRef;

			for (; *pThunkRef; ++pThunkRef, ++pFuncRef)
			{
				if (IMAGE_SNAP_BY_ORDINAL(*pThunkRef))
				{
					*pFuncRef = reinterpret_cast<ULONG_PTR>(_GetProcAddress(hDll, reinterpret_cast<char*>(*pThunkRef & 0xFFFF)));
				}
				else
				{
					auto * pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pBase + (*pThunkRef));
					*pFuncRef = reinterpret_cast<ULONG_PTR>(_GetProcAddress(hDll, pImport->Name));
				}
			}
			++pImportDescr;
		}
	}



DWORD Injector::GetProcessIdByWindow(const std::tstring& Name)
{
	// Find window
	HWND MyWnd = FindWindow(NULL,Name.c_str());
	if (!MyWnd)
		throw std::runtime_error("Could not find window.");

	// Get process ID from window
	DWORD ProcID;
	GetWindowThreadProcessId(MyWnd,&ProcID);
	if (!ProcID)
		throw std::runtime_error("Could not get process id from window.");

	// Return process id
	return ProcID;
}


if (g_pEngine->IsInGame())
	{
		g_pLocalEntity = g_pEngine->GetLocal();

		Features::RenderESP(Renderer, g_pNkContext);

		if (Globals::PressedKeys[VK_RBUTTON])
			Features::DoAimbot();

		g_pLocalEntity->NoRecoil();
		g_pLocalEntity->NoSpread();
		g_pLocalEntity->NoReload();
		g_pLocalEntity->SetGlow();
		g_pLocalEntity->SetSpeed();
		g_pLocalEntity->SetFOV();
	}


typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SysInterval, UINT Flags);
BOOL CALLBACK EnumWindowCallback(HWND hWnd, LPARAM lParam);
HWND GetMainWindowHwnd(unsigned long lProcessId);


void OnProjectileUpdate(Projectile* unk) {
	if (!unk)
		return;

	if(!settings::weapon::magic_bullet)
		return Update(unk);

	base_player* owner = (base_player*)safe_read(unk + 0xD0, DWORD64);
	if (!owner)
		return;

	if (owner->is_local_player()) {
		bool ret = false;
		if (get_isAlive((base_projectile*)unk)) {
			for (; unk->IsAlive(); unk->UpdateVelocity(0.03125f, unk, ret)) {
				if (ret) {
					break;
				}

				if (unk->launchTime() <= 0) {
					break;
				}

				float time = get_time();

				if (time - unk->launchTime() < unk->traveledTime() + 0.03125f) {
					break;
				}
			}
		}
		else {
			Retire(unk);
		}
	}
}

Logger::Logger(wstring filename)
	:logText(NULL), DoLog(false), filename(filename)
{
}


Logger::~Logger()
{
	if (logText)
		CloseHandle(logText);
}

void Logger::startLog()
{
	if (!logText) {
		wstring filepathBase = L"C:\\Users\\Hunter\\Desktop\\Logfiles\\" + filename + L".txt";
		logText = CreateFileW(filepathBase.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	DoLog = true;
}



