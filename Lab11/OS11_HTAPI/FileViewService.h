#pragma once
#include "pch.h"

#define INVALID_ADDR_FILE_VIEW NULL

namespace Core
{
	struct FileViewService
	{
		LPVOID mapFileView(HANDLE hFileMapping);
		BOOL unmapFileView(LPVOID addr);
		BOOL flushFileViewToDrive(LPVOID addr, DWORD memoryToFlush);
	};
}