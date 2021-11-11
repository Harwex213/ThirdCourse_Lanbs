#include <Windows.h>
#include <iostream>
#include <ctime>
#define SECOND 10000000

using namespace std;

HANDLE timer = 0;
long long i = 0;

DWORD WINAPI ChildThread()
{
    while (true)
    {
        i++;
    }
    return 0;
}

int main()
{
    timer = CreateWaitableTimer(NULL, FALSE, NULL);
    long long it = - 0 * SECOND;

    if (!SetWaitableTimer(timer, (LARGE_INTEGER*)&it, 3000, NULL, NULL, FALSE))
    {
        throw "Error SetWaitableTimer";
    }

    DWORD ChildId = 0;
    HANDLE hChild = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ChildThread, NULL, NULL, &ChildId);

    for (int j = 0; j < 5; j++)
    {
        WaitForSingleObject(timer, INFINITE);
        cout << "i: " << i << endl;
    }

    WaitForSingleObject(hChild, 0);
}