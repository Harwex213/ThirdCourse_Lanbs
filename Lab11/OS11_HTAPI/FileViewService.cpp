#include "pch.h"
#include "FileViewService.h"

namespace Core
{
	LPVOID FileViewService::mapFileView(HANDLE hFileMapping)
	{
		return MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	}

	BOOL FileViewService::unmapFileView(LPVOID addr)
	{
		return UnmapViewOfFile(addr);
	}

	BOOL FileViewService::flushFileViewToDrive(LPVOID addr, DWORD memoryToFlush)
	{
		return FlushViewOfFile(addr, memoryToFlush);
	}
}