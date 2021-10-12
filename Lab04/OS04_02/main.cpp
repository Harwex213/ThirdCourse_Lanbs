#include <Windows.h>
#include <iostream>

DWORD WINAPI OS04_02_T1()
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    for (int i = 0; i < 50; i++)
    {
        std::cout << "-----OS_04_02_T1-----" << std::endl;
        std::cout << "pid: " << processId << std::endl;
        std::cout << "tid: " << threadId << std::endl;
        std::cout << "---------------------" << std::endl;
        Sleep(1000);
    }

    return 0;
}

DWORD WINAPI OS04_02_T2()
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    for (int i = 0; i < 125; i++)
    {
        std::cout << "-----OS_04_02_T2-----" << std::endl;
        std::cout << "pid: " << processId << std::endl;
        std::cout << "tid: " << threadId << std::endl;
        std::cout << "---------------------" << std::endl;
        Sleep(1000);
    }

    return 0;
}

int main()
{
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    DWORD threadId_OS04_02_T1 = NULL;
    DWORD threadId_OS04_02_T2 = NULL;

    HANDLE handle_OS04_02_T1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OS04_02_T1, NULL, 0, &threadId_OS04_02_T1);
    HANDLE handle_OS04_02_T2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OS04_02_T2, NULL, 0, &threadId_OS04_02_T2);

    for (int i = 0; i < 100; i++)
    {
        std::cout << "--------Parent thread--------" << std::endl;
        std::cout << "process id: "			<< processId << std::endl;
        std::cout << "parent thread id: "		<< threadId<< std::endl;
        std::cout << "child thread id T1: "	<< threadId_OS04_02_T1 << std::endl;
        std::cout << "child thread id T2: "	<< threadId_OS04_02_T2 << std::endl;
        std::cout << "-----------------------------" << std::endl;
        Sleep(1000);
    }

    WaitForSingleObject(handle_OS04_02_T1, INFINITE);
    CloseHandle(handle_OS04_02_T1);

    WaitForSingleObject(handle_OS04_02_T2, INFINITE);
    CloseHandle(handle_OS04_02_T2);

    system("pause");
    return 0;
}