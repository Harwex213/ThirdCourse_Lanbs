#pragma once
#include "pch.h"
#include "FileService.h"

namespace Core
{
	HANDLE FileService::createFile(const char* filePath)
	{
		return CreateFileA(
			filePath,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL);
	}

	HANDLE FileService::openFile(const char* filePath)
	{
		return CreateFileA(
			filePath,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
	}

	BOOL FileService::closeFile(HANDLE handle)
	{
		return CloseHandle(handle);
	}
	BOOL FileService::writeToFile(HANDLE hFile, LPVOID memoryToWrite, DWORD amount)
	{
		return WriteFile(hFile, memoryToWrite, amount, NULL, NULL);
	}
}