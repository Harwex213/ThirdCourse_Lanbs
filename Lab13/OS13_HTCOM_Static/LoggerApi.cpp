#include "pch.h"
#include "LoggerApi.h"

void setLogger(std::string loggerPath)
{
    HANDLE hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, std::to_string(GetCurrentProcessId()).c_str());
    if (hFileMapping == NULL)
    {
        return;
    }
    LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (addr == NULL)
    {
        return;
    }
    strcpy_s((char*)addr, loggerPath.size() + 1, loggerPath.c_str());
}
