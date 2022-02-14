#pragma once
#include <string>
#include "Element.h"
#include "HtHandle.h"

namespace HT
{
	DWORD CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength);
	DWORD CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength);
	int TruncateStrByMax(std::string& truncatedStr, const char* str, int strLength, int maxStrLength);
	std::string GetFileName(const char* filename);
	std::string GetFilePath(const char* filename);
	std::string TimeToLocalString(time_t time);
}