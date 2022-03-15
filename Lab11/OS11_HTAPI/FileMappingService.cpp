#include "pch.h"
#include "FileMappingService.h"

namespace Core
{
	HANDLE Core::FileMappingService::createFileMapping(HANDLE hFile, DWORD memoryToAlloc, const char* fileMappingName)
	{
		return CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, fileMappingName);
	}

	HANDLE FileMappingService::openFileMapping(const char* fileMappingName)
	{
		return OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, fileMappingName);
	}

	BOOL Core::FileMappingService::closeFileMapping(HANDLE handle)
	{
		return CloseHandle(handle);
	}
}