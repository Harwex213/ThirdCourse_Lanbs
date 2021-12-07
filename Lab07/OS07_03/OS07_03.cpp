#include <iostream>
#include "windows.h"

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

void main()
{
	LPCWSTR exePath = L"D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab07\\x64\\Debug\\OS07_03x.exe";
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION procAInfo, procBInfo;
	CreateChildProcess("A", exePath, startupInfo, procAInfo);
	CreateChildProcess("B", exePath, startupInfo, procBInfo);

	HANDLE hm = CreateMutex(NULL, FALSE, L"OS07_mutex");

	for (int i = 0; i <= 90; i++)
	{
		if (i == 30)
		{
			WaitForSingleObject(hm, INFINITE);
		}

		cout << "Parent process: " << "pid-" << GetCurrentProcessId() << ": " << i << "\n";
		Sleep(100);

		if (i == 60)
		{
			ReleaseMutex(hm);
			Sleep(100);
			WaitForSingleObject(hm, INFINITE);
		}

		if (i == 61)
		{
			ReleaseMutex(hm);
		}
	}

	WaitForSingleObject(&procAInfo, INFINITE);
	CloseHandle(procAInfo.hProcess);

	WaitForSingleObject(&procBInfo, INFINITE);
	CloseHandle(procBInfo.hProcess);

	CloseHandle(hm);
}