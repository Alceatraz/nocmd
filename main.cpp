#include <windows.h>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

//= ====================================================================================================================

void entry();

//= ====================================================================================================================

extern "C" {

void WinMainCRTStartup() {
    entry();
}

}

//= ====================================================================================================================

typedef struct TaskInfo {
    boolean verbose;
    boolean waiting;
    boolean logging;
} TaskInfo;

//= ====================================================================================================================

void entry() {

    //= ================================================================================================================
    //= Parse command line
    //= ================================================================================================================

    wchar_t *arguments = GetCommandLineW();

    //= ========================================================================
    //= Trim argv[0]

    while (arguments[0] != ' ') {
        if (arguments[0] == 0) {
            MessageBoxW(
                    nullptr,
                    L"nohup.exe -wsx foo bar\r\nx: show verbose message\r\nw: waiting for subprocess exit\r\ns: redirect stdout and stderr to file",
                    L"ERROR: Usage incorrect - No command body",
                    MB_OK
            );
            ExitProcess(-1);
        }
        arguments++;
    }

    //= ========================================================================
    //= Trim spaces to argv[1]

    while (arguments[0] == ' ') {
        arguments++;
    }

    if (arguments[0] == 0) {
        MessageBoxW(
                nullptr,
                L"nohup.exe -wsx foo bar\r\nx: show verbose message\r\nw: waiting for subprocess exit\r\ns: redirect stdout and stderr to file",
                L"ERROR: Usage incorrect - No command body or empty",
                MB_OK
        );
        ExitProcess(-1);
    }

    //= ========================================================================
    //= Parse options

    TaskInfo taskInfo;

    taskInfo.verbose = FALSE;
    taskInfo.waiting = FALSE;
    taskInfo.logging = FALSE;

    if (arguments[0] == '-') {

        arguments++;

        while (arguments[0] != ' ') {

            switch (arguments[0]) {

                case 'x':
                    taskInfo.verbose = TRUE;
                    break;
                case 's':
                    taskInfo.logging = TRUE;
                case 'w':
                    taskInfo.waiting = TRUE;
                    break;
            }

            arguments++;
        }

        arguments++;
    }

    //= ========================================================================
    //= Remain command line as commands - Don't touch

    if (lstrlenW(arguments) == 0) {
        MessageBoxW(
                nullptr,
                L"nohup.exe -wsx foo bar\r\nx: show verbose message\r\nw: waiting for subprocess exit\r\ns: redirect stdout and stderr to file",
                L"ERROR: Usage incorrect - No command body or empty",
                MB_OK
        );
        ExitProcess(-1);
    }

    //= ================================================================================================================
    //= Config process
    //= ================================================================================================================

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW;

    si.cb = sizeof(si);

    //= ================================================================================================================
    //= Start process and/or waiting
    //= ================================================================================================================

    boolean createProcess = CreateProcessW(
            nullptr,
            arguments,
            nullptr,
            nullptr,
            FALSE,
            CREATE_NEW_CONSOLE,
            nullptr,
            nullptr,
            &si,
            &pi
    );

    DWORD exitCode = 0;

    if (createProcess) {

        if (taskInfo.waiting) {
            WaitForSingleObject(pi.hProcess, INFINITE);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

    } else {

        exitCode = GetLastError();

        MessageBoxW(
                nullptr,
                L"FATAL: Can't create child process",
                L"FATAL: Can't create child process",
                MB_OK
        );

    }

    ExitProcess(exitCode);

}


//= ============================================================================