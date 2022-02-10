#pragma once
#include "Element.h"
#include "HtHandle.h"

namespace HT
{
	DWORD CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength);
	DWORD CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength);
	int TruncateStrByMax(char* truncatedStr, const char* str, int strLength, int maxStrLength);
}