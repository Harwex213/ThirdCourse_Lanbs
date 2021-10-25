#include <iostream>
#include <synchapi.h>
#include "windows.h"
#include <dirent.h>
#include <bitset>

using namespace std;

int main(int argc, char* argv[]) {
    HANDLE processId = GetCurrentProcess();
    DWORD mask;
    DWORD priority_1;
    DWORD priority_2;
    char *end;

    if (argc > 3) {
        mask = strtol(argv[1], &end, 10);
        priority_1 = strtol(argv[2], &end, 10);
        priority_2 = strtol(argv[3], &end, 10);
    } else {
        cout << "Parameters must be: affinity mask, priority 1 process, priority 2 process";
        system("pause");
        exit(-1);
    }

    if(SetProcessAffinityMask(processId, mask)) {
        ULONG_PTR processAffinityMask;
        ULONG_PTR systemAffinityMask;

        if(GetProcessAffinityMask(processId, &processAffinityMask, &systemAffinityMask)) {
            std::cout << "process mask: " << std::showbase << std::bitset<12>(processAffinityMask) << std::endl;
        }
    }

    char* cwd = _getcwd(nullptr, 0);
    string workingDirectory = cwd;

    string  OS05_02_1_str = workingDirectory + "\\OS05_02_1.exe";
    string  OS05_02_2_str = workingDirectory + "\\OS05_02_2.exe";

    LPCSTR OS05_02_1 = OS05_02_1_str.c_str();
    LPCSTR OS05_02_2 = OS05_02_2_str.c_str();

    STARTUPINFO startupinfoOne;
    STARTUPINFO startupinfoTwo;
    PROCESS_INFORMATION processInformationOne;
    PROCESS_INFORMATION processInformationTwo;

    ZeroMemory(&startupinfoOne, sizeof(STARTUPINFO)); startupinfoOne.cb = sizeof(STARTUPINFO);
    ZeroMemory(&startupinfoTwo, sizeof(STARTUPINFO)); startupinfoTwo.cb = sizeof(STARTUPINFO);

    if(CreateProcess(OS05_02_1, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE | priority_1, NULL, NULL, &startupinfoOne, &processInformationOne)) {
        cout << "CREATED PROCESS " << OS05_02_1 << endl;
    } else {
        cout << "NOT CREATED PROCESS " << OS05_02_1 << endl;
    }

    if(CreateProcess(OS05_02_2, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE | priority_2, NULL, NULL, &startupinfoTwo, &processInformationTwo)) {
        cout << "CREATED PROCESS " << OS05_02_2 << endl;
    } else {
        cout << "NOT CREATED PROCESS " << OS05_02_2 << endl;
    }

    WaitForSingleObject(processInformationOne.hProcess, INFINITE);
    WaitForSingleObject(processInformationTwo.hProcess, INFINITE);
    CloseHandle(processInformationOne.hProcess);
    CloseHandle(processInformationTwo.hProcess);
    return 0;
}