#include "Loader.hpp"

Logger::Logger(wstring filename)
    : logText(NULL), DoLog(false), filename(filename)
{
}

void Logger::startLog()
{
    if (!logText) {
        wstring filepathBase = L"C:\\Users\\Hunter\\Desktop\\Logfiles\\" + filename + L".txt";
        logText = CreateFileW(filepathBase.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        DoLog = true;
    }
}

void Logger::stopLog()
{
    DoLog = false;
}

void Logger::logString(const std::string& explaination, const std::string& str)
{
    if (DoLog && logText) {
        std::string logme = "Log_string : " + explaination + " ! " + str + "\r\n" ;
        WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
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
