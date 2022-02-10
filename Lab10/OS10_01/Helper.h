#pragma once
#include "Element.h"
#include "HtHandle.h"

namespace HT
{
	int CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength);
	int CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength);
	int TruncateStrByMax(char* truncatedStr, const char* str, int strLength, int maxStrLength);
	void GetTruncatedKeyPayload(char* key, char* payload, const Element* element, const HTHANDLE* htHandle);
}