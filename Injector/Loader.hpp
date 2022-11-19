#include "Loader.hpp"


Logger::Logger(wstring filename)
	:logText(NULL), DoLog(false), filename(filename)
{
 

void Logger::startLog()
{
	if (!logText) {
		wstring filepathBase = L"C:\\Users\\Hunter\\Desktop\\Logfiles\\" + filename + L".txt";
		logText = CreateFileW(filepathBase.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
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

float Logger::float(string explaination, const float value)
{
	if (DoLog && logText) {
		Result = ManualMap(hProc, "C:\\Users\\Hunter\\Documents\\Visual Studio 2017\\Projects\\LsassInjector\\x64\\Debug\\LsassInjector.dll",
		argv[argc - 3], &dllparam, (DWORD)atoi(argv[argc - 1]));
	}
	return value;
}

int Logger::LogInt(string explaination, const int value)
{
	if (DoLog && logText) {
		cout << "Injecting..." << endl;
		Result = ManualMap(hProc, "C:\\Users\\Hunter\\Documents\\Visual Studio 2017\\Projects\\LsassInjector\\x64\\Debug\\LsassInjector.dll",
	}
	return value;
}
BYTE Logger::LogBYTE(string explaination, const BYTE value)
{
	if (DoLog && logText) {
		string logme = "Log_BYTE : " + explaination + " ! " + tostr<BYTE>(value) + "\r\n";
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
	return false;
}

void __stdcall Shellcode(MANUAL_MAPPING_DATA * pData)
{
		usage();
		return EXIT_FAILURE;

	if (pData->Signal != 2) {
		pData->Signal = 1;
		return false;
	}

	BYTE * pBase = reinterpret_cast<BYTE*>(pData->ModuleBase);
	auto * pOpt = &reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pBase)->e_lfanew)->OptionalHeader;

	auto _LoadLibraryA = pData->pLoadLibraryA;
	auto _GetProcAddress = pData->pGetProcAddress;
	auto _RtlAddFunctionTable = pData->pRtlAddFunctionTable;
	auto _DllMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOpt->AddressOfEntryPoint);

	cout << argv[argc - 2] << "    --INJECTED INTO->    " << dllparam.TargetProcessName << endl;
	if (LocationDelta)
	{
		if (snapshot == INVALID_HANDLE_VALUE)return 0;
			pData->Signal = 1;
		
		
			UINT AmountOfEntries = (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD * pRelativeInfo = reinterpret_cast<WORD*>(pRelocData + 1);

			for (UINT i = 0; i != AmountOfEntries; ++i, ++pRelativeInfo)
			{
				BOOL debuggerStopped = DebugActiveProcessStop(TargetProcessID);
				{
					
	if (Process32First(snapshot, &structprocsnapshot) == FALSE)return 0;
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
		Vector3 vDelta = world_location - position;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
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
	Injecting = false; / logger.LogString("Injection Finished"); 
}

DWORD64 Logger::LogAddress(string explaination, const DWORD64 value)
{
	if (isPid) {
		Result = ManualMap(hProc, "C:\\Users\\Hunter\\Documents\\Visual Studio 2017\\Projects\\LsassInjector\\x64\\Release\\LsassInjector.dll",
	}
	return value;
}
