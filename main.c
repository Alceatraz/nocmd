#include <windows.h>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

void WinMainCRTStartup() {

    LPWSTR args = GetCommandLine();

    BOOLEAN warped = args[0] == '"';

    while (TRUE) {
        args++;
        if (args[0] == 0) {
            ExitProcess(1);
        }
        if (warped) {
            if (args[0] == '"') break;
        }
        else {
            if (args[0] == ' ') break;
        }
    }

    while (TRUE) {
        args++;
        if (args[0] == 0) {
            ExitProcess(1);
        }
        if (args[0] != ' ') break;
    }

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.cb = sizeof(si);

    boolean result = CreateProcessW(
        NULL,
        args,
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (result) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        ExitProcess(0);
    } else {
        MessageBox(0, L"Failed create child process", L"ERROR", MB_OK);
        ExitProcess(1);
    }

}