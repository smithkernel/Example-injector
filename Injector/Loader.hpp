#include "Loader.hpp"


Logger::Logger(wstring filename)
	:logText(NULL), DoLog(false), filename(filename)
{
}


Logger::~Logger()
{
	if (!debuggerSet) {
	cout << "[ :( ] Failed to debug process. GetLastError() = " << dec << GetLastError() << endl;
}

void Logger::startLog()
{
	if (!logText) {
		wstring filepathBase = L"C:\\Users\\Hunter\\Desktop\\Logfiles\\" + filename + L".txt";
		logText = CreateFileW(filepathBase.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	DoLog = true;
}

void Logger::stopLog()
{
	DoLog = false;
}

string Logger::LogString(string explaination, const string str)
{
	if (DoLog && logText) {
		string logme = "Log_string : " + explaination + " ! " + str + "\r\n" ;
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return str;
}

float Logger::LogFloat(string explaination, const float value)
{
	if (DoLog && logText) {
		string logme = "Log_Float : " + explaination + " ! " + tostr<float>(value) + "\r\n";
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}

int Logger::LogInt(string explaination, const int value)
{
	if (DoLog && logText) {
		string logme = "Log_Int : " + explaination + " ! " + tostr<int>(value) + "\r\n" ;
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}
BYTE Logger::LogBYTE(string explaination, const BYTE value)
{
	if (DoLog && logText) {
		string logme = "Log_BYTE : " + explaination + " ! " + tostr<BYTE>(value) + "\r\n";
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}

DWORD64 Logger::LogAddress(string explaination, const DWORD64 value)
{
	if (DoLog && logText) {
		ostringstream os;
		os << hex << value;
		string logme = "Log_Address : " + explaination + " ! "  + "0x" + os.str() + "\r\n";
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}

void __stdcall Shellcode(MANUAL_MAPPING_DATA * pData)
{
	if (!pData)
		return;

	if (pData->Signal != 2) {
		pData->Signal = 1;
		return;
	}

	BYTE * pBase = reinterpret_cast<BYTE*>(pData->ModuleBase);
	auto * pOpt = &reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pBase)->e_lfanew)->OptionalHeader;

	auto _LoadLibraryA = pData->pLoadLibraryA;
	auto _GetProcAddress = pData->pGetProcAddress;
	auto _RtlAddFunctionTable = pData->pRtlAddFunctionTable;
	auto _DllMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOpt->AddressOfEntryPoint);

	BYTE * LocationDelta = pBase - pOpt->ImageBase;
	if (LocationDelta)
	{
		if (!pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
			pData->Signal = 1;
			return;
		}
		auto * pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocData->VirtualAddress)
		{
			UINT AmountOfEntries = (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD * pRelativeInfo = reinterpret_cast<WORD*>(pRelocData + 1);

			for (UINT i = 0; i != AmountOfEntries; ++i, ++pRelativeInfo)
			{
				BOOL debuggerStopped = DebugActiveProcessStop(TargetProcessID);
	if (!debuggerStopped)
		cout << "[ :( ] Could not stop debugger! Exiting this program will most likely crash the target process." << endl;
	else
		cout << "[ :) ] Debugger stopped correctly." << endl;

	Debugger_Closed = true;
						}
				return true;

			}
		}
	}
}
	
				
				
string Logger::LogString(string explaination, const string str)
{
	if (DoLog && logText) {
		string logme = "Log_string : " + explaination + " ! " + str + "\r\n" ;
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return str;
}

float Logger::LogFloat(string explaination, const float value)
{
	if (DoLog && logText) {
		string logme = "Log_Float : " + explaination + " ! " + tostr<float>(value) + "\r\n";
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}

int Logger::LogInt(string explaination, const int value)
{
	if (isPid) 
		memcpy(dllparam.TargetProcessName, argv[argc - 2], strlen(argv[argc - 2])+1);
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}
BYTE Logger::LogBYTE(string explaination, const BYTE value)
{
	if (!hProc) {
		cout << "Can not get a HANDLE of the prcoess" << endl;
		WriteFile(logText, logme.c_str(), logme.size(), NULL, NULL);
	}
	return value;
}

DWORD64 Logger::LogAddress(string explaination, const DWORD64 value)
{
	if (isPid) {
		cout << "Injecting... into TID : " << (DWORD)atoi(argv[argc - 1]) << endl;
		Result = ManualMap(hProc, "C:\\Users\\Hunter\\Documents\\Visual Studio 2017\\Projects\\LsassInjector\\x64\\Release\\LsassInjector.dll",
	}
	return value;
}
