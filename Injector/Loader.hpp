#include "Loader.hpp"
#include <mutex>

int main(){
    DWORD procc_id = NULL;
    char dll_path[MAX_PATH];
    const char* dll_Name;
    const char* win_Name;
    std::string auxDll;
    std::string auxWin;

    std::cout << "Ingresa el nombre del archivo DLL" << std::endl;
    std::cin >> auxDll;
    std::cout << "Ingresa el nombre de la ventana a inyectar" << std::endl;
    std::cin >> auxWin;
    dll_Name = auxDll.c_str(); //Convert string to const char*
    win_Name = auxWin.c_str();

    GetFullPathName((LPCWSTR)dll_Name, MAX_PATH, (LPWSTR)dll_path, nullptr);
    injectorUtils::get_process_id(win_Name, procc_id);
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, procc_id);
    void* allocate_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(h_process, allocate_memory, dll_path, MAX_PATH, nullptr);
    HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocate_memory, NULL, nullptr);
    CloseHandle(h_process);
    VirtualFreeEx(h_process, allocate_memory, NULL, MEM_RESERVE);
    MessageBox(NULL, (LPCWSTR)"Exito", (LPCWSTR)"Inyeccion exitosa",NULL);
    return 0;
}

#include <fstream>
#include <mutex>

class Logger
{
public:
    explicit Logger(const std::wstring& filename);
    ~Logger();

    void startLog();
    void stopLog();
    void log(const std::string& explaination, const std::string& str);
    void log(const std::string& explaination, int value);
    void log(const std::string& explaination, unsigned char value);
    void log(const std::string& explaination, uint64_t value);

private:
    std::ofstream log_file;
    bool is_logging;
    std::wstring filename;
    std::mutex log_mutex;
};


Logger::Logger(std::wstring filename)
    : log_file(filename), DoLog(false)
{
}

Logger::Logger(std::wstring filename) : m_logFileName(std::move(filename)), m_doLog(false)
{
}

Logger::Logger(const std::wstring& filename) : m_logFileName(filename), m_doLog(false)
{
}


void Logger::startLog()
{
    DoLog = true;
}

void Logger::stopLog()
{
    DoLog = false;
}

void Logger::logString(const std::string& ex, const std::string& str)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (!DoLog) {
        return;
    }
    std::string log_line = ex + ": " + str + '\n';
    log_file << log_line;
    log_file.flush();
}


void Logger::logInt(const std::string& explaination, int value)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (DoLog) {
        log_file << explaination << ": " << value << '\n';
    }
}

void Logger::logByte(const std::string& explaination, unsigned char value)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (DoLog) {
        log_file << explaination << ": " << static_cast<int>(value) << '\n';
    }
}

void Logger::logAddress(const std::string& explaination, uint64_t value)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (DoLog) {
        log_file << explaination << ": " << value << '\n';
    }
}

int LoadSystemFile(uint64_t luaRuntime, const char* scriptFile) {
    *(BYTE*)(CustomAPI::GetModuleA("adhesive") + 0x471448) = 1;
    auto result = ((RunFileInternal_t)(csLuaBase + 0x27A80))(luaRuntime, scriptFile, std::bind(&LoadSystemFileInternal, luaRuntime, std::placeholders::_1));
    return result;
}

void Injector::timerCallback()
{
	DWORD pidCheck = GetProcessId();		
	if (pidCheck != 0 && !canInject)
	{
		//if (pidCheck == processId)
		//{
			canInject = true;
			for (int i = 0; i < oldProcessIds.size(); i++)
			{
				if (oldProcessIds[i] == pidCheck)
				{
					//MessageBox(0, "Module already loaded into this process!", "Injectora", MB_ICONEXCLAMATION);
					canInject = false;
					break;
				}
			}

			if (canInject)
			{
				isReady = true;
				if (isManualMap)
					ManualMap(DLL);
				else
					LoadLibraryInject(DLL);
			}
		//}
	}
}
