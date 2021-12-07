#include <Windows.h>
#include <iostream>

using namespace std;

void main()
{
	HANDLE hm = OpenMutex(SYNCHRONIZE, FALSE, L"OS07_mutex");
	if (hm == NULL)
	{
		cout << "Mutex is failed on open\n";
	}
	else
	{
		cout << "Mutex is open\n";
	}

	DWORD pid = GetCurrentProcessId();

	for (int i = 0; i <= 90; i++)
	{
		if (i == 30)
		{
			WaitForSingleObject(hm, INFINITE);
		}

		cout << "process: " << "pid-" << GetCurrentProcessId() << ": " << i << "\n";
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

	CloseHandle(hm);
}