#include "Loader.hpp"
#include <iostream>
#include <string>
#include <Windows.h>

// A namespace to keep helper functions
namespace injectorUtils {
    // Helper function to retrieve the process ID of a given window
    void get_process_id(const char* window_name, DWORD& process_id) {
        HWND hwnd = FindWindowA(NULL, window_name);
        if (hwnd == NULL) {
            std::cerr << "Failed to find the target window." << std::endl;
            exit(EXIT_FAILURE);
        }
        GetWindowThreadProcessId(hwnd, &process_id);
    }
}

int main() {
    DWORD procc_id = 0;
    char dll_path[MAX_PATH];
    std::string auxDll;
    std::string auxWin;

    std::cout << "Enter the name of the DLL file: ";
    std::cin >> auxDll;
    std::cout << "Enter the name of the window to inject: ";
    std::cin >> auxWin;
    const char* dll_Name = auxDll.c_str(); // Convert string to const char*
    const char* win_Name = auxWin.c_str();

    GetFullPathNameA(dll_Name, MAX_PATH, dll_path, NULL);
    injectorUtils::get_process_id(win_Name, procc_id);
    HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procc_id);
    if (h_process == NULL) {
        std::cerr << "Failed to open the target process." << std::endl;
        exit(EXIT_FAILURE);
    }
    void* allocate_memory = VirtualAllocEx(h_process, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (allocate_memory == NULL) {
        std::cerr << "Failed to allocate memory in the target process." << std::endl;
        CloseHandle(h_process);
        exit(EXIT_FAILURE);
    }
    if (WriteProcessMemory(h_process, allocate_memory, dll_path, MAX_PATH, NULL) == FALSE) {
        std::cerr << "Failed to write to the process memory." << std::endl;
        VirtualFreeEx(h_process, allocate_memory, 0, MEM_RELEASE);
        CloseHandle(h_process);
        exit(EXIT_FAILURE);
    }
    HANDLE h_thread = CreateRemoteThread(h_process, NULL, 0, (LPTHREAD_START_ROUTINE) LoadLibraryA, allocate_memory, 0, NULL);
    if (h_thread == NULL) {
        std::cerr << "Failed to create remote thread." << std::endl;
        VirtualFreeEx(h_process, allocate_memory, 0, MEM_RELEASE);
        CloseHandle(h_process);
        exit(EXIT_FAILURE);
    }
    WaitForSingleObject
    VirtualFreeEx(h_process, allocate_memory, 0, MEM_RELEASE);
    CloseHandle(h_thread);
    CloseHandle(h_process);

    std::cout << "Injection successful." << std::endl;
    return 0;
}


#include <fstream>
#include <mutex>

class Logger
{
public:
    explicit Logger(const std::wstring& file_path);
    ~Logger();

    void start();
    void stop();
    void logString(const std::string& description, const std::string& str);
    void logInt(const std::string& description, int value);
    void logUnsignedChar(const std::string& description, unsigned char value);
    void logUInt64(const std::string& description, uint64_t value);

private:
    std::ofstream log_file;
    bool is_logging_enabled;
    std::wstring file_path;
    std::mutex mutex;

    void checkLogging();
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


void print_params(const t_params_struct& p)
{
    std::cout << "myDec: [" << std::dec << p.myDec << "] = [0x" << std::hex << p.myDec << "]\n";
    std::cout << "myHex: [0x" << std::hex << p.myHex << "] = [" << std::dec << p.myHex << "]\n";
    std::cout << "myBool: [" << std::boolalpha << p.myBool << "]\n";
    std::cout << "myABuf: [" << (p.myABuf ? p.myABuf : "(null)") << "]\n";
    std::wcout << L"myWBuf: [" << (p.myWBuf ? p.myWBuf : L"(null)") << L"]\n";
    std::cout << "myEnum: [" << static_cast<std::underlying_type_t<decltype(p.myEnum)>>(p.myEnum) << "]\n";
}



class Logger {
public:
    Logger(const std::string& fileName) : log_file(fileName, std::ios::app) {
        if (!log_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + fileName);
        }
    }

    void logInt(const std::string& explanation, int value) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (doLog) {
            log_file << explanation << ": " << value << '\n';
        }
    }

class Logger {
public:
    Logger(const std::string& log_filename) : log_file(log_filename, std::ios::out | std::ios::app) {}

    void logByte(const std::string& explanation, unsigned char value) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (doLog && log_file) {
            log_file << explanation << ": " << static_cast<int>(value) << '\n';
            log_file.flush();
        }
    }

    void logString(const std::string& explanation, const std::string& value) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (doLog && log_file) {
            log_file << explanation << ": " << value << '\n';
            log_file.flush();
        }
    }

    void enableLogging() {
        std::lock_guard<std::mutex> lock(log_mutex);
        doLog = true;
    }

    void disableLogging() {
        std::lock_guard<std::mutex> lock(log_mutex);
        doLog = false;
    }

private:
    std::ofstream log_file;
    std::mutex log_mutex;
    bool doLog = true;
};

void Logger::logAddress(const std::string& explaination, uint64_t value)
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (logging) {
        std::time_t time = std::time(nullptr);
        std::time localTime = std::localtime(&time);
        char timeString[20];
        std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);
        log_file << "[" << timeString << "] " << explaination << ": " << value << '\n';
    }
}


int LoadSystemFile(uint64_t luaRuntime, const std::string& scriptFile) {
    if (CustomAPI::ChangeMemoryValue(CustomAPI::GetModuleA("adhesive") + 0x471448, 1) == 0) {
        return RunFileInternal(luaRuntime, LoadSystemFileInternal);
    }
    return -1;
}


void Injector::timerCallback()
{
    DWORD processId = GetProcessId();
    if (processId == 0) {
        // Error handling: Failed to get process ID
        return;
    }

    // Check if process has already been injected
    if (std::find(oldProcessIds.begin(), oldProcessIds.end(), processId) != oldProcessIds.end()) {
        // Process already injected, do nothing
        return;
    }

    // Process not yet injected, set up for injection
    oldProcessIds.push_back(processId);

    // Call the appropriate injection method
    bool injectionSuccess = false;
    if (Manual) {
        injectionSuccess = ManualMap(dllPath);
    }
    else {
        injectionSuccess = LoadLibraryInject(dllPath);
    }

    // Check if injection was successful
    if (!injectionSuccess) {
        // Error handling: Injection failed
        return;
    }

    // Injection successful
    isReady = true;
}

