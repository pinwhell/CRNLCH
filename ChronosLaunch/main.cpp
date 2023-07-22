#include "ChronosLaunch.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (int ret = ChronosLaunch::Run(argc, argv))
    {
        std::cerr << ChronosLaunch::GetLastError() << std::endl;
        return ret;
    }

    return 0;
}
