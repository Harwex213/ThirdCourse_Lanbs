#pragma once
#include "pch.h"
#include "Values.h"

#define ALREADY_MAPPED_ERROR "Storage file already mapped."
#define ALREADY_UNMAPPED_ERROR "Storage file already unmapped."
#define CREATE_FILE_STORAGE_ERROR "Cannot create file storage."
#define OPEN_FILE_STORAGE_ERROR "Cannot open file storage."
#define CREATE_FILE_MAPPING_ERROR "Cannot create mapping of file storage."
#define OPEN_FILE_MAPPING_ERROR "Cannot open mapping of file storage."
#define MAP_VIEW_OF_FILE_ERROR "Cannot map view of file storage"
#define UNMAP_VIEW_OF_FILE_ERROR "Cannot unmap view of file storage."
#define FLUSH_VIEW_OF_FILE_ERROR "Cannot flush view of file storage."

struct StorageFileService
{
	StorageFileService();

private:
	HANDLE hFile;
	HANDLE hFileMapping;
	bool isMapped;

public:
	LPVOID CreateStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc);
	LPVOID LoadStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc);
	LPVOID OpenStorage(const char filePath[FILEPATH_SIZE]);
	void ForceCloseStorage(LPVOID addr, DWORD storageMemory);
	void CloseStorage(LPVOID addr, DWORD storageMemory);
};
