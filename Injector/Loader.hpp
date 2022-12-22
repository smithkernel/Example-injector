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

class Logger
{
public:
    Logger(wstring filename);
    void startLog();
    void stopLog();
    void logString(const std::string& explaination, const std::string& str);
    void logInt(const std::string& explaination, int value);
    void logByte(const std::string& explaination, unsigned char value);
    void logAddress(const std::string& explaination, uint64_t value);

private:
    // RAII wrapper for HANDLEs that automatically closes the handle when it goes out of scope
    struct unique_handle
    {
        HANDLE handle;
        unique_handle(HANDLE h = INVALID_HANDLE_VALUE) : handle(h) {}
        ~unique_handle() { if (handle != INVALID_HANDLE_VALUE) CloseHandle(handle); }
        operator HANDLE() const { return handle; }
    };

    unique_handle logText;
    bool DoLog;
    wstring filename;
    std::mutex log_mutex; // mutex to synchronize access to the log file
};

Logger::Logger(wstring filename)
    : logText(INVALID_HANDLE_VALUE), DoLog(false), filename(filename)
{
}

void Logger::startLog()
{
    if (logText == INVALID_HANDLE_VALUE)
    {
        wstring filepathBase = L"C:\\Users\\Hunter\\Desktop\\Logfiles\\" + filename + L".txt";
        logText = unique_handle(CreateFileW(filepathBase.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
        if (logText == INVALID_HANDLE_VALUE)
        {
            cout << "Failed to create log file: " << GetLastError() << endl;
            return;
        }
        DoLog = true;
    }
}

void Logger::stopLog()
{
    DoLog = false;
}

void Logger::logString(const std::string& explaination, const std::string& str)
{
    if (DoLog && logText != INVALID_HANDLE_VALUE)
    {
        std::lock_guard<std::mutex> lock(log_mutex); // lock mutex to synchronize access to the log file
        std::string logme = "Log_string : " + explaination + " ! " + str + "\r\n";
        WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
        FlushFileBuffers(logText); // flush log file buffer to disk
    }
}

void Logger::logInt(const std::string& explaination, int value)
{
    if (DoLog && logText) {
        std::ostringstream os;
        os << "Log_int : " << explaination << " ! " << value << "\r\n";
        WriteFile(logText, os.str().c_str(), os.str().size(), NULL, NULL);
    }
}

void Logger::logByte(const std::string& explaination, unsigned char value)
{
    if (DoLog && logText) {
        std::ostringstream os;
        os << "Log_BYTE : " << explaination << " ! " << static_cast<int>(value) << "\r\n";
        WriteFile(logText, os.str().c_str(), os.str().size(), NULL, NULL);
    }
}

void Logger::logAddress(const std::string& explaination, uint64_t value)
{
    if (DoLog && logText) {
        std::ostringstream os;
        os << "Log_Address : " << explaination << " ! " << std::hex << value << "\r\n";
        WriteFile(logText, os.str().c_str(), os.str().size(), NULL, NULL);
    }
}
