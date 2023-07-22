## CHRONOSLAUNCH - DLL PRELOADER LAUNCHER

------------



ChronosLaunch is a simple and friendly DLL Preloader Launcher application. It allows you to load custom DLLs into a target process before it starts executing. Use ChronosLaunch to enhance or modify the behavior of your favorite applications!
Getting Started

- Download ChronosLaunch/Build ChronosLaunch
- Prepare Your DLLs: Create or obtain the DLLs you want to inject into the target process. Ensure that your DLLs are compatible with the target process architecture (32-bit or 64-bit).
- Command Line Usage: To use ChronosLaunch, open a Command Prompt or Terminal and navigate to the directory containing the ChronosLaunch executable.

#### COMMAND SYNTAX: The command syntax is as follows:

```bash
ChronosLaunch.exe --launch <path_to_target_exe> --preload <dll1> <dll2> ... --cmdline <arguments>

- --launch: Specify the path to the target executable you want to launch.
- --preload: Provide one or more paths to the DLLs you want to inject into the target process.
- --cmdline: Optionally, pass any command-line arguments to the target executable.
```

Example Command: Let's say you want to inject "MyCustomDLL.dll" into "MyApp.exe" with the command-line argument  "arg1" to be forwarded to "MyApp.exe":

```bash
ChronosLaunch.exe --launch MyApp.exe --preload MyCustomDLL.dll --cmdline arg1
```

#### Error Handling:

    If ChronosLaunch encounters any issues, it will display helpful error messages in the console.
    If you see any errors, double-check that the target executable and DLLs exist in the specified paths.

#### Contributing:

Feel free to contribute to ChronosLaunch by reporting issues, suggesting improvements, or submitting pull requests.

#### License:

ChronosLaunch is released under the MIT License.