#include "Injector.h"
#include <memory>

bool Inject(HANDLE hProc, const char* dllPath)
{
    if (hProc == INVALID_HANDLE_VALUE)
        return false;

    size_t dllPathLen = strlen(dllPath);

    uintptr_t remoteDllPathStrEntry = (uintptr_t)VirtualAllocEx(hProc, NULL, dllPathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (remoteDllPathStrEntry == 0)
        return false;

    if (WriteProcessMemory(hProc, (LPVOID)remoteDllPathStrEntry, (void*)dllPath, dllPathLen, nullptr) == false)
    {
        VirtualFreeEx(hProc, (LPVOID)remoteDllPathStrEntry, NULL, MEM_RELEASE);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)remoteDllPathStrEntry, NULL, NULL);

    WaitForSingleObject(hThread, INFINITE);

    if (hThread == NULL)
    {
        VirtualFreeEx(hProc, (LPVOID)remoteDllPathStrEntry, NULL, MEM_RELEASE);
        return false;
    }

    CloseHandle(hThread);
    VirtualFreeEx(hProc, (LPVOID)remoteDllPathStrEntry, NULL, MEM_RELEASE);
    return true;
}
