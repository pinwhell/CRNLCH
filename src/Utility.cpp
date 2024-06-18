#include <Utility.h>
#include <filesystem>
#include <sstream>

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

void LaunchProcess(const char* processPath, const std::vector<std::string>& cmdline, bool suspended, const char* _workingDir, HANDLE* outhProc, HANDLE* outhThread)
{
    // Convert the command-line arguments to a single string separated by spaces
    std::string commandLine = std::string(processPath);
    std::string launchDir = std::filesystem::path(processPath).parent_path().string();
    std::string workingDir = _workingDir ? _workingDir : launchDir;

    for (const auto& arg : cmdline) {
        commandLine += " ";
        commandLine += arg;
    }

    // Process information
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Startup information
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Create the process in suspended mode if required
    DWORD creationFlags = CREATE_DEFAULT_ERROR_MODE;
    if (suspended) {
        creationFlags |= CREATE_SUSPENDED;
    }

    if (!CreateProcessA(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, FALSE, creationFlags, NULL, workingDir.c_str(), &si, &pi))
    {
        DWORD error = GetLastError();

        // Buffer to hold the error message
        LPVOID lpMsgBuf;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf,
            0, NULL);

        // Construct the error message
        std::ostringstream oss;
        oss << "CreateProcessA failed with error " << error << ": " << (char*)lpMsgBuf;

        // Free the buffer allocated by FormatMessage
        LocalFree(lpMsgBuf);

        // Throw the constructed message as an exception or handle it as needed
        throw std::runtime_error(oss.str());
    }

    // The process was successfully created
    if (outhProc)
        *outhProc = pi.hProcess;

    if (outhThread)
        *outhThread = pi.hThread;
}