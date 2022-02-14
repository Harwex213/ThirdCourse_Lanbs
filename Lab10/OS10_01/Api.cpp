#include <iostream>
#include <Windows.h>
#include "Api.h"
#include "Helper.h"
#include "Element.h"
#include "HashTable.h"
#include <thread>

namespace HT
{
	void StartIntervalSnap(HTHANDLE* htHandle, std::atomic<bool>* isIntervalSnapOn)
	{
		if (isIntervalSnapOn->load(std::memory_order_seq_cst))
		{
			do
			{
				std::this_thread::sleep_for(std::chrono::seconds(htHandle->secSnapshotInterval));
			} while (isIntervalSnapOn->load(std::memory_order_seq_cst) && Snap(htHandle));
		}

		delete isIntervalSnapOn;
	}

	void CreateDirectoryForSnaps(const char fileName[CHAR_MAX_LENGTH])
	{
		std::string directoryToCreate = GetFilePath(fileName) + "/";
		directoryToCreate += SNAPSHOT_DIRECTORY_NAME;
		CreateDirectoryA(directoryToCreate.c_str(), NULL);
	}

	HTHANDLE* Create(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		HANDLE hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// TODO: fill error;
			return NULL;
		}

		DWORD memoryToAlloc = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
		HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, 0);
		if (hFileMapping == NULL)
		{
			// TODO: fill error;
			return NULL;
		}

		LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addr == NULL)
		{
			// TODO: fill error;
			return NULL;
		}

		ZeroMemory(addr, memoryToAlloc);

		HTHANDLE* htHandle = new(addr) HTHANDLE(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
		htHandle->hFile = hFile;
		htHandle->hFileMapping = hFileMapping;
		htHandle->addr = addr;
		htHandle->tableMemorySize = memoryToAlloc;

		htHandle->StartSnapInterval();

		std::thread startIntervalSnap(StartIntervalSnap, htHandle, htHandle->isIntervalSnapOn);
		startIntervalSnap.detach();
		CreateDirectoryForSnaps(fileName);

		return htHandle;
	}

	HTHANDLE* Open(const char fileName[CHAR_MAX_LENGTH])
	{
		HANDLE hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			// TODO: fill error;
			return NULL;
		}

		HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 0, 0);
		if (hFileMapping == NULL)
		{
			// TODO: fill error;
			return NULL;
		}

		HANDLE addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addr == NULL)
		{
			// TODO: fill error;
			return NULL;
		}

		HTHANDLE* htHandle = (HTHANDLE*)addr;
		htHandle->hFile = hFile;
		htHandle->hFileMapping = hFileMapping;
		htHandle->addr = addr;

		htHandle->SetFileName(fileName);
		htHandle->InitParsedFileName(fileName);
		htHandle->StartSnapInterval();

		std::thread startIntervalSnap(StartIntervalSnap, htHandle, htHandle->isIntervalSnapOn);
		startIntervalSnap.detach();
		CreateDirectoryForSnaps(fileName);

		return htHandle;
	}

	BOOL Snap(HTHANDLE* htHandle)
	{
		if (!FlushViewOfFile(htHandle->addr, htHandle->tableMemorySize))
		{
			// TODO: fill error;
			return false;
		}

		htHandle->OnSnap();
		std::string snapFilename = htHandle->GenerateSnapFilename();

		HANDLE hSnapFile = CreateFileA(
			snapFilename.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL);
		if (hSnapFile == INVALID_HANDLE_VALUE)
		{
			// TODO: fill error;
			return false;
		}

		if (!WriteFile(hSnapFile, htHandle->addr, htHandle->tableMemorySize, NULL, NULL))
		{
			// TODO: fill error;
			return false;
		}

		if (!CloseHandle(hSnapFile))
		{
			// TODO: fill error;
			return false;
		};

		return true;
	}

	BOOL Close(HTHANDLE* htHandle)
	{
		HANDLE hFileMapping = htHandle->hFileMapping;
		HANDLE hFile = htHandle->hFile;

		htHandle->FinishSnapInterval();
		if (!Snap(htHandle))
		{
			return false;
		}

		htHandle->ClearParsedFileName();
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
		std::string key;
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);
		std::string payload;
		int payloadLength = TruncateStrByMax(payload, element->getPayload(), element->payloadLength, htHandle->maxPayloadLength);

		LPVOID elementAddr = FindUnallocatedElementAddr(htHandle, key.c_str());
		if (elementAddr == NULL)
		{
			// TODO: fill error;
			return false;
		}

		Element* elementToInsert = new(elementAddr) Element();
		LPVOID keyAddr = (char*)elementAddr + sizeof(Element);
		LPVOID payloadAddr = (char*)keyAddr + htHandle->maxKeyLength;

		elementToInsert->setKeyPointer(keyAddr, keyLength);
		elementToInsert->setKey(key.c_str(), keyLength);
		elementToInsert->setPayloadPointer(payloadAddr, payloadLength);
		elementToInsert->setPayload(payload.c_str(), payloadLength);

		htHandle->currentSize++;

		return true;
	}

	BOOL Update(HTHANDLE* htHandle, const Element* oldElement, const void* newPayload, int newPayloadlength)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, oldElement->getKey(), oldElement->keyLength, htHandle->maxKeyLength);
		std::string payload;
		int payloadLength = TruncateStrByMax(payload, (const char*)newPayload, newPayloadlength, htHandle->maxPayloadLength);

		Element* elementToUpdate = FindElementAddr(htHandle, key.c_str());
		if (elementToUpdate == NULL)
		{
			// TODO: fill error;
			return false;
		}

		elementToUpdate->setPayload(payload.c_str(), payloadLength);

		return true;
	}

	BOOL Delete(HTHANDLE* htHandle, const Element* element)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);
		Element* elementToDelete = FindElementAddr(htHandle, key.c_str());
		if (elementToDelete == NULL)
		{
			// TODO: fill error;
			return false;
		}

		elementToDelete->isDeleted = true;

		htHandle->currentSize--;

		return true;
	}

	Element* Get(HTHANDLE* htHandle, const Element* element)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);

		Element* elementToGet = FindElementAddr(htHandle, key.c_str());
		if (elementToGet == NULL)
		{
			// TODO: fill error;
		}

		return elementToGet;
	}

	char* GetLastError(HTHANDLE* htHandle)
	{
		return htHandle->lastErrorMessage;
	}

	void Print(const Element* element)
	{
		if (element == NULL)
		{
			printf_s("Printing elemenet. NULL\n");
			return;
		}
		printf_s("Printing element. %s: %s\n", element->getKey(), element->getPayload());
	}

	void PrintAllElements(HTHANDLE* htHandle)
	{
		for (int i = 0; i < htHandle->capacity; i++)
		{
			Element* elementAddr = htHandle->GetElementAddr(i);
			if (*(int*)elementAddr == NULL || elementAddr->isDeleted)
			{
				Print(NULL);
				continue;
			}
			Print(elementAddr);
		}
	}
}