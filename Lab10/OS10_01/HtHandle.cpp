#include <ctime>
#include "HtHandle.h"
#include "Helper.h"

namespace HT
{
	HTHANDLE::ParsedFileName::ParsedFileName(const char fileName[CHAR_MAX_LENGTH])
	{
		this->fileName = GetFileName(fileName);
		this->filePath = GetFilePath(fileName);
	}

	HTHANDLE::HTHANDLE()
	{
		initDefault();
	}

	HTHANDLE::HTHANDLE(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		initDefault();

		this->capacity = capacity;
		this->secSnapshotInterval = secSnapshotInterval;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		elementMemorySize = CalcElementMaxSizeMemory(maxKeyLength, maxPayloadLength);
		SetFileName(fileName);
		InitParsedFileName(fileName);
	}

	void HTHANDLE::initDefault()
	{
		hFile = NULL;
		hFileMapping = NULL;
		addr = NULL;

		snapLastTime = NULL;
		parsedFileName = NULL;
		isIntervalSnapOn = NULL;

		capacity = 1;
		secSnapshotInterval = 3;
		maxKeyLength = 5;
		maxPayloadLength = 10;
		currentSize = 0;
		currentSnap = 0;
		elementMemorySize = CalcElementMaxSizeMemory(maxKeyLength, maxPayloadLength);
		tableMemorySize = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
		SetFileName(defaultFileName);
		InitParsedFileName(defaultFileName);
		lastErrorMessage[0] = '\0';
	}

	Element* HTHANDLE::GetElementAddr(int index)
	{
		if (addr == NULL)
		{
			// TODO: fill error
			return NULL;
		}
		return (Element*)((char*)addr + sizeof(HTHANDLE) + elementMemorySize * index);
	}

	void HTHANDLE::CorrectElementPointers(LPVOID elementAddr)
	{
		Element* element = (Element *) elementAddr;
		LPVOID keyAddr = (char*)elementAddr + sizeof(Element);
		LPVOID payloadAddr = (char*)keyAddr + maxKeyLength;
		element->setKeyPointer(keyAddr, element->keyLength);
		element->setPayloadPointer(payloadAddr, element->payloadLength);
	}

	void HTHANDLE::InitParsedFileName(const char fileName[CHAR_MAX_LENGTH])
	{
		ClearParsedFileName();
		parsedFileName = new ParsedFileName(fileName);
	}
	
	void HTHANDLE::ClearParsedFileName()
	{
		if (parsedFileName)
		{
			delete parsedFileName;
			parsedFileName = NULL;
		}
	}

	void HTHANDLE::SetFileName(const char fileName[CHAR_MAX_LENGTH])
	{
		strcpy_s(this->fileName, strlen(fileName) + 1, fileName);
	}

	std::string HTHANDLE::GenerateSnapFilename()
	{
		std::string snapFilename = parsedFileName->filePath + "/";
		snapFilename += SNAPSHOT_DIRECTORY_NAME;
		snapFilename += "/" + parsedFileName->fileName + "-";
		snapFilename += std::to_string(currentSnap++);
		snapFilename += ".ht";

		return snapFilename;
	}

	void HTHANDLE::OnSnap()
	{
		time(&snapLastTime);
	}

	void HTHANDLE::StartSnapInterval()
	{
		isIntervalSnapOn = new std::atomic<bool>(true);
	}

	void HTHANDLE::FinishSnapInterval()
	{
		isIntervalSnapOn->store(false, std::memory_order_seq_cst);
	}
}