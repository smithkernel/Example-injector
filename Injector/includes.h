#pragma once

#include <Windows.h>

class Injector : private Timer
{
public:
	Injector();
	Injector(String nameIn);
	Injector(String nameIn, bool autoInjectIn, bool closeOnInjectIn);

	~Injector();

	Injector(Injector& other);

	BOOL CheckValidProcessExtension(const char* name);
	DWORD GetProcessId();
	void EnableDebugPriv() { Utils::SetDebugPrivilege(TRUE); };

	void SetProcessInformation(ProcessInfo processInfo) 
	{ 
		processName = processInfo.processName; 
		processId = processInfo.processId; 
	}

	void SetDLLName(String dllname) { DLL = dllname; }
	void SetAutoInject(bool bAutoInj) { autoInject = bAutoInj; }
	void SetManualMap(bool bManualMap) { isManualMap = bManualMap; }
	void SetCloseOnInject(bool bCloseOnInj) { closeOnInject = bCloseOnInj; }

	void timerCallback();
	void beginTimer() { startTimer(750); }
	void terminateTimer() { stopTimer(); }
	bool isTimerAlive() { return isTimerRunning(); }

	bool Setup();

	HRESULT ManualMap(String filePath);
	BOOL LoadLibraryInject(String filePath);

private:
    bool m_autoInject;
    bool m_closeOnInject;
    bool m_hasInjected;
    bool m_isManualMap;
    bool m_isReady;
    bool m_canInject;
    std::string m_DLL;
    std::vector<DWORD> m_oldProcessIds;
    std::string m_processName;
    DWORD m_processId;
    HANDLE m_processHandle;
    CRemoteLoader m_remoteLoader;

    HMODULE m_hNtdll;
    tNTQSI m_fnQSI;

public:
    DLLInjector();
    ~DLLInjector();

    bool Init();
    bool InjectDLL();
    bool CloseOnInject();
    bool HasInjected();
    bool IsManualMap();
    bool IsReady();
    bool CanInject();
    std::string GetDLLName();
    std::vector<DWORD> GetOldProcessIds();
    std::string GetProcessName();
    DWORD GetProcessId();
    HANDLE GetProcessHandle();
};
