#include <Windows.h>
#include <iostream>

using namespace std;

LPCWSTR exePath = L"D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab08\\x64\\Debug\\OS08_04x.exe";
STARTUPINFO startupInfo;

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

DWORD WINAPI ChildThreadProcessA()
{
	PROCESS_INFORMATION procAInfo;
	CreateChildProcess("A", exePath, startupInfo, procAInfo);

	WaitForSingleObject(procAInfo.hProcess, 2000);
	TerminateProcess(procAInfo.hProcess, 0);
	CloseHandle(procAInfo.hProcess);

	return 0;
}

DWORD WINAPI ChildThreadProcessB()
{
	PROCESS_INFORMATION procBInfo;
	CreateChildProcess("B", exePath, startupInfo, procBInfo);

	WaitForSingleObject(procBInfo.hProcess, 3000);
	TerminateProcess(procBInfo.hProcess, 0);
	CloseHandle(procBInfo.hProcess);

	return 0;
}

void main()
{
	DWORD ChildId1 = 0;
	DWORD ChildId2 = 0;
	HANDLE hChild1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChildThreadProcessA, NULL, NULL, &ChildId1);
	HANDLE hChild2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChildThreadProcessB, NULL, NULL, &ChildId2);

	WaitForSingleObject(hChild1, INFINITE);
	WaitForSingleObject(hChild2, INFINITE);
}