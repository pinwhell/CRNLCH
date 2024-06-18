#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <functional>

void StartSuspendedCallbackExecuteProcess(const std::string& exe, const std::vector<std::string>& cmdline, const std::string& workingDir, std::function<void(HANDLE hProc)> callback, bool bDetach = true);

