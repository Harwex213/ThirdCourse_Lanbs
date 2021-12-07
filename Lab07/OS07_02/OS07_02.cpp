#include <Windows.h>
#include <iostream>

using namespace std;

CRITICAL_SECTION cs;

void CriticalLoop(const char* threadName, DWORD tid)
{
	for (int i = 0; i <= 90; i++)
	{
		if (i == 30)
		{
			EnterCriticalSection(&cs);
		}

		cout << threadName << " Thread: " << "tid-" << tid << ": " << i << "\n";
		Sleep(100);

		if (i == 60)
		{
			LeaveCriticalSection(&cs);
			Sleep(100);
			EnterCriticalSection(&cs);
		}

		if (i == 61)
		{
			LeaveCriticalSection(&cs);
		}
	}
}

DWORD WINAPI ChildThread_A()
{
	CriticalLoop("A", GetCurrentThreadId());
	return 0;
}

DWORD WINAPI ChildThread_B()
{
	CriticalLoop("B", GetCurrentThreadId());
	return 0;
}

void main()
{
	DWORD ChildA_Id = NULL;
	DWORD ChildB_Id = NULL;
	
	InitializeCriticalSection(&cs);

	HANDLE hChildA = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChildThread_A, NULL, 0, &ChildA_Id);
	HANDLE hChildB = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChildThread_B, NULL, 0, &ChildB_Id);
	
	CriticalLoop("Main", GetCurrentThreadId());

	WaitForSingleObject(hChildA, INFINITE);
	CloseHandle(hChildA);
	WaitForSingleObject(hChildB, INFINITE);
	CloseHandle(hChildB);

	DeleteCriticalSection(&cs);
}