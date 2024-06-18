#include <CRNLCH.h>
#include <filesystem>
#include <Utility.h>

void StartSuspendedCallbackExecuteProcess(const std::string& exe, const std::vector<std::string>& cmdline, const std::string& workingDir, std::function<void(HANDLE hProc)> callback, bool bDetach)
{
    HANDLE outhProc;
    HANDLE outhThread;
    LaunchProcess(exe.c_str(), cmdline, false, workingDir.c_str(), &outhProc, &outhThread);
    callback(outhProc);
    ResumeThread(outhThread);
    if(!bDetach)
        WaitProcessDie(outhProc);
    CloseHandle(outhThread);
    CloseHandle(outhProc);
}