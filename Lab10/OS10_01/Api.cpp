#include <iostream>
#include <Windows.h>
#include "Api.h"
#include "Helper.h"
#include "Element.h"
#include "HashTable.h"

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

		LPVOID elementAddr;

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

		HANDLE hFileMapping = htHandle->hFileMapping;
		HANDLE hFile = htHandle->hFile;
		if (!UnmapViewOfFile(htHandle->addr))
		{
			// TODO: fill error;
			return false;
		}

		if (!CloseHandle(hFileMapping))
		{
			// TODO: fill error;
			return false;
		}

		if (!CloseHandle(hFile))
		{
			// TODO: fill error;
			return false;
		}

		return true;
	}

	BOOL Insert(HTHANDLE* htHandle, const Element* element)
	{
		char* key = new char[htHandle->maxKeyLength];
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);
		char* payload = new char[htHandle->maxPayloadLength];
		int payloadLength = TruncateStrByMax(payload, element->getPayload(), element->payloadLength, htHandle->maxPayloadLength);

		LPVOID elementAddr = FindUnallocatedElementAddr(htHandle, key);
		if (elementAddr == NULL)
		{
			delete[] key;
			delete[] payload;

			// TODO: fill error;
			return false;
		}

		Element* elementToInsert = new(elementAddr) Element();
		LPVOID keyAddr = (char*)elementAddr + sizeof(Element);
		LPVOID payloadAddr = (char*)keyAddr + htHandle->maxKeyLength;

		elementToInsert->key = new(keyAddr) char[htHandle->maxKeyLength];
		elementToInsert->setKey(key, keyLength);
		elementToInsert->payload = new(payloadAddr) char[htHandle->maxPayloadLength];
		elementToInsert->setPayload(payload, payloadLength);

		htHandle->currentSize++;

		delete[] key;
		delete[] payload;
		return true;
	}

	BOOL Update(HTHANDLE* htHandle, const Element* oldElement, const void* newPayload, int newPayloadlength)
	{
		char* key = new char[htHandle->maxKeyLength];
		int keyLength = TruncateStrByMax(key, oldElement->getKey(), oldElement->keyLength, htHandle->maxKeyLength);
		char* payload = new char[htHandle->maxPayloadLength];
		int payloadLength = TruncateStrByMax(payload, (const char*)newPayload, newPayloadlength, htHandle->maxPayloadLength);

		Element* elementToUpdate = FindElementAddr(htHandle, key);
		if (elementToUpdate == NULL || elementToUpdate->isDeleted)
		{
			delete[] key;
			delete[] payload;

			// TODO: fill error;
			return false;
		}

		elementToUpdate->setPayload(payload, payloadLength);

		delete[] key;
		delete[] payload;
		return true;
	}

	BOOL Delete(HTHANDLE* htHandle, const Element* element)
	{
		char* key = new char[htHandle->maxKeyLength];
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);
		Element* elementToDelete = FindElementAddr(htHandle, key);
		if (elementToDelete == NULL)
		{
			delete[] key;

			// TODO: fill error;
			return false;
		}

		elementToDelete->isDeleted = true;

		htHandle->currentSize--;

		delete[] key;
		return true;
	}

	Element* Get(HTHANDLE* htHandle, const Element* element)
	{
		char* key = new char[htHandle->maxKeyLength];
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);
		Element* elementToGet = FindElementAddr(htHandle, key);
		if (elementToGet == NULL)
		{
			// TODO: fill error;
		}

		delete[] key;
		return elementToGet;
	}

	char* GetLastError(HTHANDLE* htHandle)
	{
		return htHandle->lastErrorMessage;
	}

	void Print(const Element* element)
	{
		printf_s("%s: %s\n", element->getKey(), element->getPayload());
	}
}