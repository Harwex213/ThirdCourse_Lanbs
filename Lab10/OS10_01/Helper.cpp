#include "Helper.h"
#include "HashTable.h"

namespace HT
{
	int CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength)
	{
		return sizeof(HTHANDLE) + htCapacity * (sizeof(Element) + elMaxKeyLength + elMaxPayloadLength);
	}
}