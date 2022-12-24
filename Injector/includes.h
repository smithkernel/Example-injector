#include <Windows.h>

int main() {
  // Load the module (in this case, the "kernel32.dll" library)
  HMODULE hModule = LoadLibraryA("kernel32.dll");
  if (hModule == NULL) {
    // Handle error
  }

  // Get the address of the "GetCurrentProcessId" function
  void* pFunc = GetModuleExport(hModule, "GetCurrentProcessId");
  if (pFunc == NULL) {
    // Handle error
  }

  // Cast the function pointer to the appropriate type and call it
  typedef DWORD (WINAPI *GetCurrentProcessIdFunc)();
  GetCurrentProcessIdFunc pGetCurrentProcessId = (GetCurrentProcessIdFunc)pFunc;
  DWORD processId = pGetCurrentProcessId();
  printf("Current process ID: %d\n", processId);

  return 0;
}
