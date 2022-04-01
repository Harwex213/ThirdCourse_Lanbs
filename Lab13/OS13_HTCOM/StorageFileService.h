#pragma once
#include "pch.h"

#define ALLOC_ALL_MEMORY_VALUE 0

#define INVALID_FILE_HANDLE INVALID_HANDLE_VALUE
#define INVALID_FILE_MAPPING_HANDLE NULL

#define CREATE_FILE_STORAGE_ERROR "Cannot create file storage."
#define CREATE_FILE_MAPPING_ERROR "Cannot create mapping of file storage."

struct StorageFileService
{
	StorageFileService();

private:
	HANDLE hFile;
	HANDLE hFileMapping;

public:
	LPVOID CreateStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc);
	LPVOID LoadStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc);
	LPVOID OpenStorage(const char filePath[FILEPATH_SIZE]);
};
