#include "Injector.h"
#include <memory>
#include <unordered_map>
#include <cstdint>
#include <string>
#include "LoadDllStub.h"

std::unordered_map<HANDLE, uintptr_t> gDllStubs;

uintptr_t GetLoadDllStub(HANDLE hProc)
{
    if (gDllStubs.find(hProc) != gDllStubs.end())
        return gDllStubs[hProc];

    uintptr_t stub = (uintptr_t)VirtualAllocEx(hProc, NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (stub == NULL)
        return stub;

    if (WriteProcessMemory(hProc, (LPVOID)stub, RPLdrLoadDll, 0x100, nullptr) == NULL)
    {
        VirtualFreeEx(hProc, (LPVOID)stub, NULL, MEM_RELEASE);
        return 0x0;
    }

    gDllStubs[hProc] = stub;

    return gDllStubs[hProc];
}

std::unordered_map<HANDLE, uintptr_t> gDllStubsInfo;

uintptr_t GetLoadDllStubInfo(HANDLE hProc)
{
    if (gDllStubsInfo.find(hProc) != gDllStubsInfo.end())
        return gDllStubsInfo[hProc];

    uintptr_t stubInfo = (uintptr_t)VirtualAllocEx(hProc, NULL, sizeof(LoadStubInfo), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (stubInfo == NULL)
        return stubInfo;

    LoadStubInfo dummyStubInfo;

    memset(&dummyStubInfo, 0x0, sizeof(LoadStubInfo));

    if (WriteProcessMemory(hProc, (LPVOID)stubInfo, &dummyStubInfo, sizeof(LoadStubInfo), nullptr) == NULL)
    {
        VirtualFreeEx(hProc, (LPVOID)stubInfo, NULL, MEM_RELEASE);
        return 0x0;
    }

    gDllStubsInfo[hProc] = stubInfo;

    return gDllStubsInfo[hProc];
}

bool PrepareLdrDllStubInfo(HANDLE hProc, uintptr_t stubRemoteAddr, const std::wstring& dllPath)
{
    LoadStubInfo stubInfo;

    memset(&stubInfo, 0x0, sizeof(LoadStubInfo));

    if (LdrLoadDll == nullptr)
        return false;

    stubInfo.mLdrLoadDll = LdrLoadDll;

    wcscpy_s(stubInfo.mDllPath, dllPath.data());

    if (WriteProcessMemory(hProc, (LPVOID)stubRemoteAddr, &stubInfo, sizeof(LoadStubInfo), nullptr) == NULL)
        return false;;

    return true;
}

bool PrepareLdrDllStubInfo(HANDLE hProc, uintptr_t stubRemoteAddr, const std::string& dllPath)
{
    std::wstring dllPathW = std::wstring(dllPath.begin(), dllPath.end());

    return PrepareLdrDllStubInfo(hProc, stubRemoteAddr, dllPathW);
}

bool Inject(HANDLE hProc, const char* dllPath)
{
    if (hProc == INVALID_HANDLE_VALUE)
        return false;

    auto ntdll = LoadLibraryA("ntdll.dll");

    if (ntdll == nullptr)
        return false;

    LdrLoadDll = (tLdrLoadDll)GetProcAddress(ntdll, "LdrLoadDll");

    if (LdrLoadDll == nullptr)
        return false;

    uintptr_t remoteStub = GetLoadDllStub(hProc);

    if (remoteStub == NULL)
        return false;

    uintptr_t remoteStubInfo = GetLoadDllStubInfo(hProc);

    if (remoteStubInfo == NULL)
        return false;

    if (PrepareLdrDllStubInfo(hProc, remoteStubInfo, dllPath) == false)
        return false;

    HANDLE hThread = CreateRemoteThread(hProc, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteStub, (LPVOID)remoteStubInfo, NULL, NULL);

    WaitForSingleObject(hThread, INFINITE);

    if (hThread == NULL)
        return false;

    CloseHandle(hThread);

    return true;
}
