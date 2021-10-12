#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    for (int i = 0; i < 100000; i++)
    {
        cout << "-------------" << endl;
        cout << "process Id: " << pid << endl;
        cout << "thread Id: " << tid << endl;
        cout << "-------------" << endl;
        Sleep(1000);
    }

    return 0;
}