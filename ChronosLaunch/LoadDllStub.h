#pragma once

#include <Windows.h>

typedef void(__stdcall* tLdrLoadDll)
(
    PWCHAR PathToFile,
    ULONG Flags,
    const wchar_t* ModuleFileName,
    HMODULE* ModuleHandle
    );

extern tLdrLoadDll LdrLoadDll;

struct LoadStubInfo {
    tLdrLoadDll mLdrLoadDll;
    wchar_t mDllPath[260];
};

void __stdcall RPLdrLoadDll(LoadStubInfo* ldrInfo);

