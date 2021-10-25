#include <iostream>
#include <synchapi.h>
#include "windows.h"
#include <dirent.h>
#include <bitset>

using namespace std;

char const* priorityThreadToString(int priority)
{
    switch (priority) {
        case THREAD_PRIORITY_IDLE:          return "THREAD_PRIORITY_IDLE\n";
        case THREAD_PRIORITY_LOWEST:        return "THREAD_PRIORITY_LOWEST\n";
        case THREAD_PRIORITY_BELOW_NORMAL:  return "THREAD_PRIORITY_BELOW_NORMAL\n";
        case THREAD_PRIORITY_NORMAL:        return "THREAD_PRIORITY_NORMAL\n";
        case THREAD_PRIORITY_ABOVE_NORMAL:  return "THREAD_PRIORITY_ABOVE_NORMAL\n";
        case THREAD_PRIORITY_HIGHEST:       return "THREAD_PRIORITY_HIGHEST\n";
        case THREAD_PRIORITY_TIME_CRITICAL: return "THREAD_PRIORITY_TIME_CRITICAL\n";
    }

}

DWORD WINAPI TA()
{
    for (int i = 0; i < 1000000; ++i) {
        if (i % 1000 == 0) {
            cout << "i:                     " << i << endl;
            cout << "current PID:           " << GetCurrentProcessId() << endl;
            cout << "thread ID:             " << GetCurrentThreadId() << endl;
            cout << "priority thread:       " << priorityThreadToString(GetThreadPriority(GetCurrentThread())) << endl;
            Sleep(200);
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    HANDLE process = GetCurrentProcess();
    DWORD mask;
    DWORD processPriority;
    int firstThreadPriority;
    int secondThreadPriority;
    char *end;

    if (argc > 4) {
        mask = strtol(argv[1], &end, 10);
        processPriority = strtol(argv[2], &end, 10);
        firstThreadPriority = strtol(argv[3], &end, 10);
        secondThreadPriority = strtol(argv[4], &end, 10);
    } else {
        cout << "Parameters must be: affinity mask, process priority, 1 thread priority, 2 thread priority" << endl;
        system("pause");
        exit(-1);
    }

    SetPriorityClass(process, processPriority);
    if(SetProcessAffinityMask(process, mask)) {
        ULONG_PTR processAffinityMask;
        ULONG_PTR systemAffinityMask;

        if(GetProcessAffinityMask(process, &processAffinityMask, &systemAffinityMask)) {
            std::cout << "process mask: " << std::showbase << bitset<12>(processAffinityMask) << std::endl;
        }
    }

    DWORD threadId_OS04_02_T1 = NULL;
    DWORD threadId_OS04_02_T2 = NULL;

    HANDLE handle_OS04_02_T1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TA, NULL, 0, &threadId_OS04_02_T1);
    SetThreadPriority(handle_OS04_02_T1, firstThreadPriority);

    HANDLE handle_OS04_02_T2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TA, NULL, 0, &threadId_OS04_02_T2);
    SetThreadPriority(handle_OS04_02_T2, secondThreadPriority);

    WaitForSingleObject(handle_OS04_02_T1, INFINITE);
    CloseHandle(handle_OS04_02_T1);

    WaitForSingleObject(handle_OS04_02_T2, INFINITE);
    CloseHandle(handle_OS04_02_T2);

    system("pause");
    return 0;
}