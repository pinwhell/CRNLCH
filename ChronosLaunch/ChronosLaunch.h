#pragma once

#include <string>

class ChronosLaunch
{
public:
	static bool IsLastErrorSet();
	static std::string GetLastError();
	static void SetLastError(const std::string& err);
	static void SetLastErrorIfNotSet(const std::string& err);

	static int Run(int argc, char* argv[]);
};

