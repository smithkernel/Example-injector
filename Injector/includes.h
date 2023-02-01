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
}

void MainLoop() {
	static RECT OldRect;
	ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));

	while (DirectX9Interface::Message.message != WM_QUIT) {
		if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9Interface::Message);
			DispatchMessage(&DirectX9Interface::Message);
		}
		HWND foregroundWindow = GetForegroundWindow();
		if (foregroundWindow == GameVars.gameHWND) {
			HWND tempProcessHwnd = GetWindow(foregroundWindow, GW_HWNDPREV);
			SetWindowPos(OverlayWindow::Hwnd, tempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT tempRect;
		POINT tempPoint;
		ZeroMemory(&tempRect, sizeof(RECT));
		ZeroMemory(&tempPoint, sizeof(POINT));

		GetClientRect(GameVars.gameHWND, &tempRect);
		ClientToScreen(GameVars.gameHWND, &tempPoint);

		tempRect.left = tempPoint.x;
		tempRect.top = tempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameVars.gameHWND;

		POINT tempPoint2;
		GetCursorPos(&tempPoint2);
		io.MousePos.x = tempPoint2.x - tempPoint.x;
		io.MousePos.y = tempPoint2.y - tempPoint.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].y = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			GameVars.ScreenWidth = TempRect.right;
			GameVars.ScreenHeight = TempRect.bottom;
			DirectX9Interface::pParams.BackBufferWidth = GameVars.ScreenWidth;
			DirectX9Interface::pParams.BackBufferHeight = GameVars.ScreenHeight;
			SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, GameVars.ScreenWidth, GameVars.ScreenHeight, SWP_NOREDRAW);
			DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		}
		Render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (DirectX9Interface::pDevice != NULL) {
		DirectX9Interface::pDevice->EndScene();
		DirectX9Interface::pDevice->Release();
	}
	if (DirectX9Interface::Direct3D9 != NULL) {
		DirectX9Interface::Direct3D9->Release();
	}
	DestroyWindow(OverlayWindow::Hwnd);
	UnregisterClass(OverlayWindow::WindowClass.lpszClassName, OverlayWindow::WindowClass.hInstance);
}

bool DirectXInit() {
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
		return false;
	}
