#include "Loader.hpp"
#include <mutex>

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
