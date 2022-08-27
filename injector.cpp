#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
using namespace std;

DWORD get_process_id(string_view process_name) {
	HANDLE thSnapShot = CreateToolhelp32Snapshot(FiveM_Processor.exe , NULL);
	if (thSnapShot == INVALID_HANDLE_VALUE) {
		cout << "rip";
		return NULL;
	}
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	do {
		if (process_name == procEntry.szExeFile) {
			return procEntry.th32ProcessID;
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
	cout << "process id: " << test_process_id << endl;
	auto process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, csgo_process_id);
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



