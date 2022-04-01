#include "pch.h"
#include "StorageFileService.h"

StorageFileService::StorageFileService()
{
    hFile = NULL;
    hFileMapping = NULL;
	isMapped = false;
}

LPVOID StorageFileService::CreateStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc)
{
	if (isMapped)
	{
		throw std::exception(ALREADY_MAPPED_ERROR);
	}

    this->hFile = CreateFileA(
		filePath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);
	if (this->hFile == INVALID_FILE_HANDLE)
	{
		this->hFile = NULL;
		throw std::exception(CREATE_FILE_STORAGE_ERROR);
	}

	this->hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, filePath);
	if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
	{
		this->hFileMapping = NULL;
		throw std::exception(CREATE_FILE_MAPPING_ERROR);
	}

	LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (addr == INVALID_FILE_VIEW_ADDR)
	{
		throw std::exception(MAP_VIEW_OF_FILE_ERROR);
	}

	isMapped = true;
	return addr;
}

LPVOID StorageFileService::LoadStorage(const char filePath[FILEPATH_SIZE], DWORD memoryToAlloc)
{
	if (isMapped)
	{
		throw std::exception(ALREADY_MAPPED_ERROR);
	}

	this->hFile = CreateFileA(
		filePath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (this->hFile == INVALID_FILE_HANDLE)
	{
		this->hFile = NULL;
		throw std::exception(OPEN_FILE_STORAGE_ERROR);
	}

	this->hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, filePath);
	if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
	{
		this->hFileMapping = NULL;
		throw std::exception(CREATE_FILE_MAPPING_ERROR);
	}

	LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (addr == INVALID_FILE_VIEW_ADDR)
	{
		throw std::exception(MAP_VIEW_OF_FILE_ERROR);
	}

	isMapped = true;
	return addr;
}

LPVOID StorageFileService::OpenStorage(const char filePath[FILEPATH_SIZE])
{
	if (isMapped)
	{
		throw std::exception(ALREADY_MAPPED_ERROR);
	}

	this->hFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, filePath);
	if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
	{
		this->hFileMapping = NULL;
		throw std::exception(OPEN_FILE_MAPPING_ERROR);
	}

	LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (addr == INVALID_FILE_VIEW_ADDR)
	{
		throw std::exception(MAP_VIEW_OF_FILE_ERROR);
	}

	isMapped = true;
    return addr;
}

void StorageFileService::ForceCloseStorage(LPVOID addr, DWORD storageMemory)
{
	FlushViewOfFile(addr, storageMemory);
	UnmapViewOfFile(addr);
	CloseHandle(this->hFileMapping);
	CloseHandle(this->hFile);

	this->hFile = NULL;
	this->hFileMapping = NULL;
	isMapped = false;
}

void StorageFileService::CloseStorage(LPVOID addr, DWORD storageMemory)
{
	if (isMapped == false)
	{
		throw std::exception(ALREADY_UNMAPPED_ERROR);
	}

	if (FlushViewOfFile(addr, storageMemory) == FALSE)
	{
		throw std::exception(FLUSH_VIEW_OF_FILE_ERROR);
	}
	if (UnmapViewOfFile(addr) == FALSE)
	{
		throw std::exception(UNMAP_VIEW_OF_FILE_ERROR);
	}
	if (CloseHandle(this->hFileMapping) == FALSE)
	{
		throw std::exception(FLUSH_VIEW_OF_FILE_ERROR);
	}
	if (CloseHandle(this->hFile) == FALSE)
	{
		throw std::exception(FLUSH_VIEW_OF_FILE_ERROR);
	}

	this->hFile = NULL;
	this->hFileMapping = NULL;
	isMapped = false;
}
