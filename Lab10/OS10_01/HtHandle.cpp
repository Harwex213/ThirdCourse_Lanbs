#include "HtHandle.h"

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
		strcpy_s(this->fileName, strlen(fileName) + 1, fileName);
	}

	void HTHANDLE::initDefault()
	{
		capacity = 0;
		secSnapshotInterval = 3;
		maxKeyLength = 10;
		maxPayloadLength = 50;
		strcpy_s(fileName, strlen(defaultFileName) + 1, defaultFileName);

		hFile = NULL;
		hFileMapping = NULL;
		addr = NULL;

		snapLastTime = NULL;
		lastErrorMessage[0] = '\0';
	}
}