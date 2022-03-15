#pragma once
#include "pch.h"

#define INVALID_FILE_HANDLE INVALID_HANDLE_VALUE

namespace Core
{
	struct FileService
	{
		HANDLE createFile(const char* filePath);
		HANDLE openFile(const char* filePath);
		BOOL closeFile(HANDLE handle);
		BOOL writeToFile(HANDLE hFile, LPVOID memoryToWrite, DWORD amount);
	};
}