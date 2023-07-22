#pragma once

#include <Windows.h>

bool Inject(HANDLE hProc, const char* dllPath);
