#define NOMINMAX

#include <CLI/CRNLCHCLI.h>
#include <Windows.h>
#include <CRNLCH.h>
#include <clipp.h>
#include <iostream>

int CRNLCHMain(int argc, const char** argv)
{
    try {
        std::string launchExecPath;
        std::vector<std::string> forwardArgs;
        std::string workingDir;
        std::vector<std::string> toLoadDlls;
        bool detach = false;

        auto cli = (
            clipp::option("--launch") & clipp::value("to launch .exe", launchExecPath).required(true),
            clipp::option("--cmdline") & clipp::values("forward cmline args", forwardArgs).required(false),
            clipp::option("--working-dir") & clipp::value("working directory to forward", workingDir).required(false),
            clipp::option("--detach") & clipp::value("detach from process", detach).required(false),
            clipp::option("--preload") & clipp::values("to preload dlls", toLoadDlls).required(false)
            );

        auto results = clipp::parse(argc, const_cast<char**>(argv), cli);

        if (!results ||
            launchExecPath.empty())
        {
            std::cout << clipp::make_man_page(cli) << std::endl;
            return false;
        }

        LaunchProcess(launchExecPath, forwardArgs, workingDir, toLoadDlls, detach);

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}