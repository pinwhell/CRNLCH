#include "Utils.h"

bool WaitProcessDie(size_t pid)
{
    bool result = true;

    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, (DWORD)pid);

    if (WaitProcessDie(hProcess) == false)
        result = false;

    CloseHandle(hProcess);

    return result;
}

bool WaitProcessDie(HANDLE hProcess)
{
    if (hProcess == NULL)
        return false;

    DWORD waitResult = WaitForSingleObject(hProcess, INFINITE);

    if (waitResult != WAIT_OBJECT_0)
        return false;

    return true;
}