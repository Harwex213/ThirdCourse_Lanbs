#include <iostream>
#include <Windows.h>
#include <thread>
#include "Api.h"
#include "Helper.h"
#include "Element.h"
#include "HashTable.h"

namespace HT
{
	HTHANDLE* Create(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandle = new HTHANDLE(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);

		try
		{
			WaitForSingleObject(htHandle->hMutex, INFINITE);

			htHandle->CreateHtFile();
			DWORD memoryToAlloc = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
			htHandle->CreateHtFileMapping(memoryToAlloc);
			htHandle->CreateViewOfHtFile(memoryToAlloc);
			htHandle->CreateSharedMemory();
			htHandle->LaunchIntervalSnap();
		}
		catch (const std::exception&)
		{
			delete htHandle;
			htHandle = NULL;
		}

		ReleaseMutex(htHandle->hMutex);

		return htHandle;
	}

	HTHANDLE* Open(const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandle = new HTHANDLE();

		try
		{
			WaitForSingleObject(htHandle->hMutex, INFINITE);

			htHandle->OpenHtFile();
			htHandle->OpenHtFileMapping();
			htHandle->OpenViewOfHtFile();
			htHandle->OpenSharedMemory();
			htHandle->LaunchIntervalSnap();
		}
		catch (const std::exception&)
		{
			delete htHandle;
			htHandle = NULL;
		}

		ReleaseMutex(htHandle->hMutex);

		return htHandle;
	}

	BOOL Snap(HTHANDLE* htHandle)
	{
		bool result = true;
		try
		{
			if (!isProcessFileOwner)
			{
				throw NOT_FILE_OWNER_ERROR;
			}

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
				throw CREATE_SNAPFILE_ERROR;
			}

			if (!WriteFile(hSnapFile, htHandle->addr, htHandle->tableMemorySize, NULL, NULL))
			{
				throw WRITE_SNAPFILE_ERROR;
			}

			if (!CloseHandle(hSnapFile))
			{
				throw CLOSE_SNAPFILE_ERROR;
			};

			time(&htHandle->snapLastTime);
			htHandle->isTableChanged = false;
		}
		catch (const char* error)
		{
			htHandle->SetLastError(error);
			result = false;
		}

		return result;
	}

	BOOL Close(HTHANDLE* htHandle)
	{
		HANDLE hFileMapping = htHandle->hFileMapping;
		HANDLE hFile = htHandle->hFile;

		htHandle->SetIntervalSnapOff();
		if (!Snap(htHandle))
		{
			return false;
		}

		if (!FlushViewOfFile(htHandle->addr, htHandle->tableMemorySize))
		{
			htHandle->SetLastError(FLUSH_VIEW_ERROR);
			return false;
		}

		if (!UnmapViewOfFile((LPVOID)htHandle->sharedMemory))
		{
			htHandle->SetLastError(UNMAP_VIEW_ERROR);
			return false;
		}

		if (!CloseHandle(hFileMapping))
		{
			htHandle->SetLastError(CLOSE_FILE_MAPPING_ERROR);
			return false;
		}

		if (hFile != NULL && !CloseHandle(hFile))
		{
			htHandle->SetLastError(CLOSE_FILE_ERROR);
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

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		LPVOID elementAddr = FindUnallocatedElementAddr(htHandle, key.c_str());
		if (elementAddr == NULL)
		{
			htHandle->SetLastError(INSERT_ERROR);
			ReleaseMutex(htHandle->hMutex);
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
		htHandle->isTableChanged = true;

		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	BOOL Update(HTHANDLE* htHandle, const Element* oldElement, const void* newPayload, int newPayloadlength)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, oldElement->getKey(), oldElement->keyLength, htHandle->maxKeyLength);
		std::string payload;
		int payloadLength = TruncateStrByMax(payload, (const char*)newPayload, newPayloadlength, htHandle->maxPayloadLength);

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		Element* elementToUpdate = FindElementAddr(htHandle, key.c_str());
		if (elementToUpdate == NULL)
		{
			htHandle->SetLastError(UPDATE_ERROR);
			ReleaseMutex(htHandle->hMutex);
			return false;
		}

		elementToUpdate->setPayload(payload.c_str(), payloadLength);

		htHandle->isTableChanged = true;
		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	BOOL Delete(HTHANDLE* htHandle, const Element* element)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		Element* elementToDelete = FindElementAddr(htHandle, key.c_str());
		if (elementToDelete == NULL)
		{
			htHandle->SetLastError(DELETE_ERROR);
			ReleaseMutex(htHandle->hMutex);
			return false;
		}

		elementToDelete->isDeleted = true;

		htHandle->currentSize--;
		htHandle->isTableChanged = true;
		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	Element* Get(HTHANDLE* htHandle, const Element* element)
	{
		std::string key;
		int keyLength = TruncateStrByMax(key, element->getKey(), element->keyLength, htHandle->maxKeyLength);

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		Element* elementToGet = FindElementAddr(htHandle, key.c_str());
		ReleaseMutex(htHandle->hMutex);
		if (elementToGet == NULL)
		{
			htHandle->SetLastError(GET_ERROR);
		}

		return elementToGet;
	}

	char* GetHTLastError(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return lastErrorMessage;
		}
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
			htHandle->CorrectElementPointers(elementAddr);
			Print(elementAddr);
		}
	}
}