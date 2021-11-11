#include <Windows.h>
#include <iostream>

using namespace std;

void CreateChildProcess(const char* procName, LPCWSTR& exePath, STARTUPINFO& startupAInfo, PROCESS_INFORMATION& procAInfo)
{
    ZeroMemory(&startupAInfo, sizeof(STARTUPINFO));
    startupAInfo.cb = sizeof(STARTUPINFO);

    if (CreateProcess(exePath, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupAInfo, &procAInfo))
    {
        cout << "proc " << procName << " - created\n";
    }
    else
    {
        cout << "proc " << procName << " - not created\n";
    }
}

int main()
{
    LPCWSTR exePathA = L"D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab07\\x64\\Debug\\OS07_05A.exe";
    LPCWSTR exePathB = L"D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab07\\x64\\Debug\\OS07_05B.exe";
    STARTUPINFO startupInfoA, startupInfoB;
    PROCESS_INFORMATION proccessInfoA, proccessInfoB;

    CreateChildProcess("A", exePathA, startupInfoA, proccessInfoA);
    CreateChildProcess("B", exePathB, startupInfoB, proccessInfoB);

    HANDLE he = CreateEvent(NULL, TRUE, FALSE, L"OS07_event");

    DWORD pid = GetCurrentProcessId();
    LONG prevcount = 0;

    for (int i = 0; i < 90; i++)
    {
        if (i == 15)
        {
            SetEvent(he);
        }
        cout << pid << " main " << i << endl;
        Sleep(100);
    }

    WaitForSingleObject(proccessInfoA.hProcess, INFINITE);
    WaitForSingleObject(proccessInfoB.hProcess, INFINITE);

    CloseHandle(proccessInfoA.hProcess);
    CloseHandle(proccessInfoB.hProcess);

    CloseHandle(he);
}