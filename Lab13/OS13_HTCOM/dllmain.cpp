#include "pch.h"
#include "SelfRegistration.h"

HMODULE g_hModule = NULL;
std::fstream logger;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;

        // TODO: how to set valid logger name or get it from 
        logger.open("StorageLog.txt", std::fstream::out);
        logger << "DllMain: " << "DLL_PROCESS_ATTACH" << std::endl;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        logger << "DllMain: " << "DLL_PROCESS_DETACH" << std::endl;
        logger.close();
        break;
    }
    return TRUE;
}

