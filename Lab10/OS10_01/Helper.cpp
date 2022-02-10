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
}