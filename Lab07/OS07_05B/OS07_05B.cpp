#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
    DWORD pid = GetCurrentProcessId();

    HANDLE he = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"OS07_event");
    if (he == NULL)
    {
        cout << "OS07_05B: Open Error Event" << endl;
    }
    else
    {
        cout << "OS07_05B: Open Event" << endl;
    }

    WaitForSingleObject(he, INFINITE);

    for (int i = 0; i < 90; i++)
    {
        cout << pid << " OS07_05B " << i << endl;
        Sleep(100);
    }

    CloseHandle(he);
}