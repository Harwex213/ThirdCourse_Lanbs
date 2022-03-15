#pragma once
#include "pch.h"

#define INVALID_FILE_MAPPING_HANDLE NULL
#define ALLOC_ALL_MEMORY_VALUE 0

namespace Core
{
	struct FileMappingService
	{
		HANDLE createFileMapping(HANDLE hFile, DWORD memoryToAlloc, const char* fileMappingName);
		HANDLE openFileMapping(const char* fileMappingName);
		BOOL closeFileMapping(HANDLE handle);
	};
}