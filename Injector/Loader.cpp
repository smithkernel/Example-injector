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

int main(int, char* argv[])
{
    try
    {
        // Needed to proxy SEH exceptions to C++ exceptions
        SehGuard Guard;

        // Injector version number
        const std::tstring VerNum(_T("20190819"));

        // Version and copyright output
#ifdef _WIN64
        std::tcout << _T("Injector x64 [Version ") << VerNum << _T("]") << std::endl;
#else
        std::tcout << _T("Injector x86 [Version ") << VerNum << _T("]") << std::endl;
#endif
        std::tcout << _T("Copyright (c) 2009 Cypher, 2012-2019 Nefarius. All rights reserved.") << std::endl << std::endl;

        argh::parser cmdl;

        cmdl.add_params({
            "n", "process-name",
            "w", "window-name",
            "p", "process-id"
            });

        cmdl.parse(argv);

        // Display help
        if (cmdl[{ "-h", "--help" }])
        {
            std::cout << "usage: Injector [options] [modules]" << std::endl << std::endl;
            std::cout << "  options:" << std::endl;
            std::cout << "    specify at least one of the following methods:" << std::endl;
            std::cout << "      -n, --process-name        Identify target process by process name" << std::endl;
            std::cout << "      -w, --window-name         Identify target process by window title" << std::endl;
            std::cout << "      -p, --process-id          Identify target process by numeric ID" << std::endl << std::endl;
            std::cout << "    specify at least one of the following actions:" << std::endl;
            std::cout << "      -i, --inject              Inject/load referenced module" << std::endl;
            std::cout << "      -e, --eject               Eject/unload referenced module" << std::endl << std::endl;
            std::cout << "  modules:" << std::endl;
            std::cout << "      myLib.dll [anotherLib.dll] [C:\\hooks\\yetAnotherLib.dll]" << std::endl;
            std::cout << std::endl;

            return RESULT_SUCCESS;
        }

        // Check positional parameter count
        if (cmdl.pos_args().size() <= 1)
        {
            std::tcerr << "No module name(s) or path(s) specified!" << std::endl;
            return RESULT_INVALID_COMMAND;
        }

        // Check if at least one action is specified
        if (!cmdl[{ "-i", "--inject", "-e", "--eject" }])
        {
            std::tcerr << "No action specified!" << std::endl;
            return RESULT_INVALID_COMMAND;
        }

        // Check if user wants more than we can handle ;)
        if (cmdl[{ "-i", "--inject" }] && cmdl[{ "-e", "--eject" }])
        {
            std::tcerr << "Only one action at a time allowed!" << std::endl;
            return RESULT_INVALID_COMMAND;
        }

        // Check if there's at least one process identification method specified
        if (!(cmdl({ "-n", "--process-name" }))
            && !(cmdl({ "-w", "--window-name" }))
            && !(cmdl({ "-p", "--process-id" })))
        {
            std::tcerr << "No process identifier specified!" << std::endl;
            return RESULT_INVALID_COMMAND;
        }

        // Variable to store process ID
        DWORD ProcID = 0;
        // Fully qualified module path
        std::tstring ModulePath;

        // Temporary place for argument
        std::string optArg;

        // Find and inject via process name
        if (cmdl({ "-n", "--process-name" }))
        {
            optArg = cmdl({ "-n", "--process-name" }).str();
            // Attempt injection via process name
            ProcID = Injector::Get()->GetProcessIdByName(utf8_to_wstr(toLower(optArg)));
        }

        // Find and inject via window name
        if (cmdl({ "-w", "--window-name" }))
        {
            optArg = cmdl({ "-w", "--window-name" }).str();
            // Attempt injection via window name
            ProcID = Injector::Get()->GetProcessIdByWindow(utf8_to_wstr(optArg));
        }

        // Find and inject via process id
        if ((cmdl({ "-p", "--process-id" }) >> optArg))
        {
            // Convert PID
            ProcID = _tstoi(utf8_to_wstr(optArg).c_str());

            if (ProcID == 0)
            {
                throw std::exception("Invalid PID entered!");
            }
        }

        // Get privileges required to perform the injection
        Injector::Get()->GetSeDebugPrivilege();

        std::vector<std::wstring> modules;

        for (auto it = std::next(cmdl.pos_args().begin()); it != cmdl.pos_args().end(); ++it)
            modules.push_back(utf8_to_wstr(*it));

        // Inject action
        if (cmdl[{ "-i", "--inject" }])
        {
            for (auto& mod : modules)
            {
                if (PathIsRelative(mod.c_str()))
                {
                    ModulePath = Injector::Get()->GetPath(mod);
                }
                else
                {
                    ModulePath = mod;
                }

                // Inject module
                Injector::Get()->InjectLib(ProcID, ModulePath);
                // If we get to this point then no exceptions have been thrown so we
                // assume success.
                std::tcout << "Successfully injected module!" << std::endl;
            }
        }

        // Eject action
        if (cmdl[{ "-e", "--eject" }])
        {
            for (auto& mod : modules)
            {
                if (PathIsRelative(mod.c_str()))
                {
                    ModulePath = Injector::Get()->GetPath(mod);
                }
                else
                {
                    ModulePath = mod;
                }

                // Eject module
                Injector::Get()->EjectLib(ProcID, ModulePath);
                // If we get to this point then no exceptions have been thrown so we
                // assume success.
                std::tcout << "Successfully ejected module!" << std::endl;
            }
        }
    }
    // Catch STL-based exceptions.
    catch (const std::exception& e)
    {
        std::string TempError(e.what());
        std::tstring Error(TempError.begin(), TempError.end());
        std::tcerr << "General Error:" << std::endl
            << Error << std::endl;
        return RESULT_GENERAL_ERROR;
    }
    // Catch custom SEH-proxy exceptions.
    // Currently only supports outputting error code.
    // TODO: Convert to string and dump more verbose output.
    catch (const SehException& e)
    {
        std::tcerr << "SEH Error:" << std::endl
            << e.GetCode() << std::endl;
        return RESULT_SEH_ERROR;
    }
    // Catch any other unknown exceptions.
    // TODO: Find a better way to handle this. Should never happen anyway, but
    // you never know.
    // Note: Could use SetUnhandledExceptionFilter but would potentially be 
    // messy.
    catch (...)
    {
        std::tcerr << "Unknown error!" << std::endl;
        return RESULT_UNKNOWN_ERROR;
    }

    // Return success
    return RESULT_SUCCESS;
}

    

    using hooked_functions = std::unordered_map<uintptr_t, uintptr_t>;

class process
{
public:
	process(HANDLE handle) : handle(handle) {}
	process(uint32_t id, DWORD desired_access) : handle(safe_handle(OpenProcess(desired_access, true, id))) { }
	process() : handle(), hooks() { }

	explicit operator bool();

#pragma region Statics
	static process current_process();
	static uint32_t from_name(const std::string& process_name);
#pragma endregion

#pragma region Memory
	MEMORY_BASIC_INFORMATION virtual_query(const uintptr_t address);
	uintptr_t raw_allocate(const SIZE_T virtual_size, const uintptr_t address = 0);
	bool free_memory(const uintptr_t address);
	bool read_raw_memory(void* buffer, const uintptr_t address, const SIZE_T size);
	bool write_raw_memory(void* buffer, const SIZE_T size, const uintptr_t address);
	bool virtual_protect(const uintptr_t address, uint32_t protect, uint32_t* old_protect);

	uintptr_t map(memory_section& section);

	template <class T>
	inline uintptr_t allocate_and_write(const T& buffer)
	{
		auto buffer_pointer = allocate(buffer);
		write_memory(buffer, buffer_pointer);
		return buffer_pointer;
	}

	template <class T>
	inline uintptr_t allocate()
	{
		return raw_allocate(sizeof(T));
	}

	template<class T>
	inline bool read_memory(T* buffer, const uintptr_t address)
	{
		return read_raw_memory(buffer, address, sizeof(T));
	}

	template<class T>
	inline bool write_memory(const T& buffer, const uintptr_t address)
	{
		uint32_t old_protect;
		this->virtual_protect(address, PAGE_EXECUTE_READWRITE, &old_protect);
		auto result = write_raw_memory(reinterpret_cast<unsigned char*>(const_cast<T*>(&buffer)), sizeof(T), address);
		this->virtual_protect(old_protect, PAGE_EXECUTE_READWRITE, &old_protect);

		return result;
	}
    
    
    
