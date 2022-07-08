#include "includes.h"
#include "functions.h"


using namespace std;

const string VERSION = "1.2";
const string AUTHOR = "h311d1n3r";
const string REQUIRED_LIBS[] = {"asmjit.dll", "LetsHook.dll"};
const string LIBS_PATH = "./libs/";


ANDLE openProcess(int pid) {
    return OpenProcess(PROCESS_ALL_ACCESS, false, pid);
}

LPVOID allocateMemory(HANDLE process, string dll_path) {
    return VirtualAllocEx(process, NULL, dll_path.size() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void writeDLLPath(HANDLE process, string dll_path, LPVOID addr) {
    WriteProcessMemory(process, addr, dll_path.c_str(), dll_path.size() + 1, NULL);
}

HANDLE loadDLL(HANDLE process, LPVOID addr) {
    return CreateRemoteThread(process, nullptr, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, addr, NULL, nullptr);
}


void Cleanup(const std::string message) {
    std::cout << message << std::endl;
    system("pause");
    ExitProcess(0);
}

std::string RandomString(const size_t length)
{
    std::string r;
    static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
    srand((unsigned)time(NULL) * 5);
    for (int i = 0; i < length; ++i)
        r += bet[rand() % (sizeof(bet) - 1)];
    return r;
}

}

LPVOID allocateMemory(HANDLE process, string dll_path) {
    return VirtualAllocEx(process, NULL, dll_path.size() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}



int main()
{
    SetConsoleTitleA(RandomString(26).c_str());

    DWORD ProcessId = Functions::GetProcessId("csgo.exe");

    if (!ProcessId)
        Cleanup("No ProcessId Found.");

    HANDLE Game = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
    uintptr_t ModuleBase = Functions::GetModuleBaseAddress(ProcessId, "client.dll");

    std::cout << "csgo.exe -> " << std::dec << ProcessId << std::endl;

    if (Functions::DoesFileExist("cheat.dll")) {

        if (!Functions::Internal::ExecuteBypass(Game)) {
            Cleanup("Cannot Bypass...");
        }

        if (Functions::LoadLibraryInject(ProcessId, "cheat.dll")) {

            Functions::Internal::Backup(Game);
            std::cout << "Injected" << std::endl;
            system("pause");
            ExitProcess(0);
        }
        else
        {
            Functions::Internal::Backup(Game);
            Cleanup("Injection Failed.");
        }

    }



    return 0;
}


endl;
