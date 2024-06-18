#include <CRNLCH.h>
#include <filesystem>
#include <Utility.h>
#include <dll_injector.h>

void StartSuspendedCallbackExecuteProcess(const std::string& exe, const std::vector<std::string>& cmdline, const std::string& workingDir, std::function<void(HANDLE hProc)> callback, bool bDetach)
{
    HANDLE outhProc;
    HANDLE outhThread;
    LaunchProcess(exe.c_str(), cmdline, true, workingDir.c_str(), &outhProc, &outhThread);
    callback(outhProc);
    ResumeThread(outhThread);
    if(!bDetach)
        WaitProcessDie(outhProc);
    CloseHandle(outhThread);
    CloseHandle(outhProc);
}

void LaunchProcess(const std::string& exe, const std::vector<std::string>& cmdline, const std::string& workingDir, const std::vector<std::string>& preloadDll, bool bDetach)
{
    StartSuspendedCallbackExecuteProcess(exe, cmdline, workingDir, [&](HANDLE hProc) {

        for (const std::string& dll : preloadDll)
            injectDLL(hProc, dll.c_str());

    }, bDetach);
}