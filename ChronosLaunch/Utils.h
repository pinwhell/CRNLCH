#pragma once

#include <Windows.h>

bool WaitProcessDie(size_t pid);
bool WaitProcessDie(HANDLE hProcess);