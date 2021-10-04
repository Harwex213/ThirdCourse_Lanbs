#include <iostream>
#include <synchapi.h>
#include "windows.h"
#include <dirent.h>

using namespace std;

int main() {
    char* cwd = _getcwd(nullptr, 0);
    string workingDirectory = cwd;

    string  OS03_02_1_str = workingDirectory + "\\OS03_02_1.exe";
    string  OS03_02_2_str = workingDirectory + "\\OS03_02_2.exe";

    LPCSTR OS03_02_1 = OS03_02_1_str.c_str();
    LPCSTR OS03_02_2 = OS03_02_2_str.c_str();

    STARTUPINFO startupinfoOne;
    STARTUPINFO startupinfoTwo;
    PROCESS_INFORMATION processInformationOne;
    PROCESS_INFORMATION processInformationTwo;

    ZeroMemory(&startupinfoOne, sizeof(STARTUPINFO)); startupinfoOne.cb = sizeof(STARTUPINFO);
    ZeroMemory(&startupinfoTwo, sizeof(STARTUPINFO)); startupinfoTwo.cb = sizeof(STARTUPINFO);

    if(CreateProcess(OS03_02_1,  NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupinfoOne, &processInformationOne)) {
        cout << "CREATED PROCESS " << OS03_02_1 << endl;
    } else {
        cout << "NOT CREATED PROCESS " << OS03_02_1 << endl;
    }

    if(CreateProcess(OS03_02_2, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupinfoTwo, &processInformationTwo)) {
        cout << "CREATED PROCESS " << OS03_02_2 << endl;
    } else {
        cout << "NOT CREATED PROCESS " << OS03_02_2 << endl;
    }

    for(int i = 0; i < 100; i++ ) {
        cout << "OS03_03, " << "PID: " << GetCurrentProcessId() << endl;
        Sleep(1000);
    }

    WaitForSingleObject(processInformationOne.hProcess, INFINITE);
    WaitForSingleObject(processInformationTwo.hProcess, INFINITE);
    CloseHandle(processInformationOne.hProcess);
    CloseHandle(processInformationTwo.hProcess);
    return 0;
}