#include <CLI/CRNLCHCLI.h>
#include <filesystem>

int main(int argc, const char* realArgv[])
{
	const char* argv[]{
		realArgv[0], 
		"--launch", "notepad.exe",
		"--detach", "true",
		"--preload", "Injected.dll"
	};

	return CRNLCHMain(sizeof(argv) / sizeof(argv[0]), argv);
}