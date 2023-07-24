#include "LoadDllStub.h"

tLdrLoadDll LdrLoadDll;

void __stdcall RPLdrLoadDll(LoadStubInfo* ldrInfo)
{
    HMODULE hDll;
    ldrInfo->mLdrLoadDll(nullptr, 0, ldrInfo->mDllPath, &hDll);
}