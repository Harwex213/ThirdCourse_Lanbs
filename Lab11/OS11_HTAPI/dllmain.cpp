// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "HT DLL ATTACH" << std::endl;
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "HT DLL THREAD ATTACH" << std::endl;
        break;
    case DLL_THREAD_DETACH:
        std::cout << "HT DLL THREAD DETACH" << std::endl;
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "HT DLL PROCESS DETACH" << std::endl;
        break;
    }
    return TRUE;
}

