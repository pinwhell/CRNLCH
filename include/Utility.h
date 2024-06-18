#pragma once

#include <Windows.h>
#include <vector>
#include <string>

bool WaitProcessDie(size_t pid);
bool WaitProcessDie(HANDLE hProcess);
void LaunchProcess(const char* processPath, const std::vector<std::string>& cmdline, bool suspended, const char* workingDir = nullptr, HANDLE* outhProc = nullptr, HANDLE* outhThread = nullptr);
