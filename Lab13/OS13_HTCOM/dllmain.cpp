#include "pch.h"
#include "SelfRegistration.h"

HMODULE g_hModule = NULL;
std::fstream logger;

bool initLogger()
{
    HANDLE hFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, std::to_string(GetCurrentProcessId()).c_str());
    if (hFileMapping == NULL)
    {
        return false;
    }
    LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (addr == NULL)
    {
        return false;
    }

    logger.open((char*)addr, std::fstream::out);
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        if (initLogger() == false)
        {
            logger.open("StorageLog.txt", std::fstream::out);
        }

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

