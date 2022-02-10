#include <iostream>
#include <Windows.h>
#include "Api.h"
#include "Helper.h"
#include "Element.h"

namespace HT
{
	HTHANDLE* Create(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandleTemp = new HTHANDLE();
		htHandleTemp->hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (htHandleTemp->hFile == INVALID_HANDLE_VALUE)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		int memoryToAlloc = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
		htHandleTemp->hFileMapping = CreateFileMappingA(htHandleTemp->hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, 0);
		if (htHandleTemp->hFileMapping == NULL)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		htHandleTemp->addr = MapViewOfFile(htHandleTemp->hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (htHandleTemp->addr == NULL)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		HTHANDLE* htHandle = new(htHandleTemp->addr) HTHANDLE(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
		htHandle->hFile = htHandleTemp->hFile;
		htHandle->hFileMapping = htHandleTemp->hFileMapping;
		htHandle->addr = htHandleTemp->addr;

		delete htHandleTemp;
		return htHandle;
	}

	HTHANDLE* Open(const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandleTemp = new HTHANDLE();
		htHandleTemp->hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (htHandleTemp->hFile == INVALID_HANDLE_VALUE)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		htHandleTemp->hFileMapping = CreateFileMappingA(htHandleTemp->hFile, NULL, PAGE_READWRITE, 0, 0, 0);
		if (htHandleTemp->hFileMapping == NULL)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		htHandleTemp->addr = MapViewOfFile(htHandleTemp->hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (htHandleTemp->addr == NULL)
		{
			// TODO: fill error;
			return htHandleTemp;
		}

		HTHANDLE* htHandle = (HTHANDLE*)htHandleTemp->addr;
		htHandle->hFile = htHandleTemp->hFile;
		htHandle->hFileMapping = htHandleTemp->hFileMapping;
		htHandle->addr = htHandleTemp->addr;

		delete htHandleTemp;
		return htHandle;
	}

	BOOL Snap(const HTHANDLE* htHandle)
	{
		// TODO: implement Snap
		return true;
	}

	BOOL Close(const HTHANDLE* htHandle)
	{
		if (!Snap(htHandle))
		{
			return false;
		}

		if (!UnmapViewOfFile(htHandle->addr))
		{
			// TODO: fill error;
			return false;
		}

		if (!CloseHandle(htHandle->hFileMapping))
		{
			// TODO: fill error;
			return false;
		}

		if (!CloseHandle(htHandle->hFile))
		{
			// TODO: fill error;
			return false;
		}
	}

	BOOL Insert(const HTHANDLE* hthandle, const Element* element)
	{
		// TODO: implement Update
		return true;
	}

	BOOL Update(const HTHANDLE* hthandle, const Element* oldelement, const void* newpayload, int newpayloadlength)
	{
		// TODO: implement Update
		return true;
	}

	BOOL Delete(const HTHANDLE* hthandle, const Element* element)
	{
		// TODO: implement Update
		return true;
	}

	Element* Get(const HTHANDLE* hthandle, const Element* element)
	{
		// TODO: implement Update
		return new Element();
	}

	char* GetLastError(HTHANDLE* htHandle)
	{
		return htHandle->lastErrorMessage;
	}

	void Print(const Element* element)
	{
		printf_s("%s: %s", element->getKey(), element->getPayload());
	}
}