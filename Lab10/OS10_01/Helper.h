#pragma once
#include <string>
#include "Element.h"
#include "HtHandle.h"

namespace HT
{
	DWORD CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength);
	DWORD CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength);
	std::string TruncateStrByMax(int& strLength, const char* str, int maxStrLength);
	std::string GetFileName(const char filename[CHAR_MAX_LENGTH]);
	std::string GetFilePath(const char filename[CHAR_MAX_LENGTH]);
	std::string TimeToLocalString(time_t time);
	std::string GenerateRandomString(int length);
}