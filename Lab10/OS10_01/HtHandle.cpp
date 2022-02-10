#include "HtHandle.h"
#include "Helper.h"

namespace HT
{
	HTHANDLE::HTHANDLE()
	{
		initDefault();
	}

	HTHANDLE::HTHANDLE(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[512])
	{
		initDefault();

		this->capacity = capacity;
		this->secSnapshotInterval = secSnapshotInterval;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		elementSize = CalcElementMaxSizeMemory(maxKeyLength, maxPayloadLength);
		strcpy_s(this->fileName, strlen(fileName) + 1, fileName);
	}

	void HTHANDLE::initDefault()
	{
		capacity = 0;
		secSnapshotInterval = 3;
		maxKeyLength = 10;
		maxPayloadLength = 50;
		strcpy_s(fileName, strlen(defaultFileName) + 1, defaultFileName);
		elementSize = CalcElementMaxSizeMemory(maxKeyLength, maxPayloadLength);

		hFile = NULL;
		hFileMapping = NULL;
		addr = NULL;

		snapLastTime = NULL;
		lastErrorMessage[0] = '\0';
	}

	Element* HTHANDLE::GetElement(int index)
	{
		if (addr == NULL)
		{
			// TODO: fill error
			return NULL;
		}
		return (Element*)((char*)addr + sizeof(HTHANDLE) + elementSize * index);
	}
}