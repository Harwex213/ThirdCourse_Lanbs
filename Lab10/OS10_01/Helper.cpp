#include "Helper.h"
#include "HtHandle.h"
#include "Element.h"

namespace HT
{
	int CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength)
	{
		int elementSize = CalcElementMaxSizeMemory(elMaxKeyLength, elMaxPayloadLength);
		return sizeof(HTHANDLE) + htCapacity * (elementSize);
	}

	int CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength)
	{
		return sizeof(Element) + elMaxKeyLength + elMaxPayloadLength;
	}

	int TruncateStrByMax(char* truncatedStr, const char* str, int strLength, int maxStrLength)
	{
		int newStrLength = strLength > maxStrLength ? maxStrLength : strLength;
		strcpy_s(truncatedStr, newStrLength, str);

		return newStrLength;
	}
}