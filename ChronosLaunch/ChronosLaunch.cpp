#include "ChronosLaunch.h"
#include "clipp.h"
#include "FileHelper.h"
#include <vector>
#include <string>
#include <Windows.h>
#include "Utils.h"
#include <iostream>
#include <functional>
#include "Injector.h"

std::string gLastError;

void ChronosLaunch::SetLastError(const std::string& err)
{
    gLastError = err;
}

void ChronosLaunch::SetLastErrorIfNotSet(const std::string& err)
{
    if (IsLastErrorSet() == true)
        return;

    SetLastError(err);
}

bool ChronosLaunch::IsLastErrorSet()
{
    return gLastError.empty() == false;
}

std::string ChronosLaunch::GetLastError()
{
    std::string lastErr = gLastError;

    gLastError = "";

    return lastErr;
}

bool ParseCheckCmdLine(int argc, char* argv[], std::string& outLaunch, std::vector<std::string>& outDlls, std::vector<std::string>& outLauncheeCmdlnie)
{
    auto cli = (
        clipp::option("--launch") & clipp::value("to launch .exe", outLaunch).required(true),
        clipp::option("--preload") & clipp::values("to preload dlls", outDlls).required(false),
        clipp::option("--cmdline") & clipp::values("to launch cmdline", outLauncheeCmdlnie).required(false)
        );

    auto results = clipp::parse(argc, argv, cli);

    if (!results ||
        outLaunch.empty())
    {
        std::cout << clipp::make_man_page(cli) << std::endl;
        return false;
    }

    if (FileHelper::FileExist(outLaunch) == false)
    {
        ChronosLaunch::SetLastErrorIfNotSet('\'' + outLaunch +  '\'' + " doest not exist");
        return false;
    }

    for (int i = 0; i < outDlls.size(); i++)
    {
        const std::string& currDll = outDlls[i];

        if (FileHelper::FileExist(currDll) == false)
        {
            ChronosLaunch::SetLastErrorIfNotSet('\'' + currDll + '\'' + " doest not exist");
            return false;
        }
    }

    return true;
}

bool LaunchProcess(const char* processPath, const std::vector<std::string>& cmdline, bool suspended, HANDLE* outhProc = nullptr, HANDLE* outhThread = nullptr)
{
    // Convert the command-line arguments to a single string separated by spaces
    std::string commandLine = std::string(processPath);

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

    if (CreateProcessA(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, FALSE, creationFlags, NULL, NULL, &si, &pi))
    {
        // The process was successfully created
        if(outhProc)
            *outhProc = pi.hProcess;

        if (outhThread)
            *outhThread = pi.hThread;

        return true;
    }
    else
    {
        return false;
    }
}

bool StartSuspendedCallbackExecuteProcess(const std::string& toLaunch, const std::vector<std::string>& args, std::function<bool(HANDLE hProc)> callback)
{
    HANDLE outhProc;
    HANDLE outhThread;

    if (LaunchProcess(toLaunch.c_str(), args, false, &outhProc, &outhThread) == false)
    {
        ChronosLaunch::SetLastErrorIfNotSet("Unable to start \'" + toLaunch + "\'");
        return false;
    }

    if (callback(outhProc) == false)
    {
        ChronosLaunch::SetLastErrorIfNotSet("Callback Reported Fail, Proceeding with the termination of \'" + toLaunch + "\'");

        TerminateProcess(outhProc, 0);

        CloseHandle(outhThread);
        CloseHandle(outhProc);

        return false;
    }

    //ResumeThread(outhThread);

    if (WaitProcessDie(outhProc) == false)
    {
        ChronosLaunch::SetLastErrorIfNotSet("Unable to wait \'" + toLaunch + "\' finish execution");
        return false;
    }

    CloseHandle(outhThread);
    CloseHandle(outhProc);

    return true;
    
}

int ChronosLaunch::Run(int argc, char* argv[])
{
    std::string toLaunch;
    std::vector<std::string> dlls;
    std::vector<std::string> cmdline;

    if (ParseCheckCmdLine(argc, argv, toLaunch, dlls, cmdline) == false)
        return 1;

    // At this point there is at least a process to launch, 
    // optionally dlls to load & cmdline to forward to process to launch

    bool result = StartSuspendedCallbackExecuteProcess(toLaunch, cmdline, [&](HANDLE hProc) {

        for (const std::string& dll : dlls)
        {
            if (Inject(hProc, dll.c_str()) == false)
            {
                ChronosLaunch::SetLastErrorIfNotSet("Failed Loading \'" + dll + "\' into " + "\'" + toLaunch + "\'");
                return false;
            }
        }

        return true;

        });

    if (result == false)
    {
        ChronosLaunch::SetLastErrorIfNotSet("Error happend trying to do operation with \'" + toLaunch + "\'");
        return 1;
    }

    return 0;
}