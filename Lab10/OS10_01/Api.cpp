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

	const char* TakeMapView(HANDLE& hFile, HANDLE& hFileMapping, LPVOID& addr, const char fileName[CHAR_MAX_LENGTH], DWORD memoryToAlloc = 0)
	{
		hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);

		hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, fileName);
		if (hFileMapping == NULL)
		{
			return CREATE_FILE_MAPING_ERROR;
		}

		addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addr == NULL)
		{
			return CREATE_MAP_VIEW_ERROR;
		}

		if (hFile == INVALID_HANDLE_VALUE)
		{
			hFile = NULL;
		}
		return NULL;
	}

	HTHANDLE* Create(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		HANDLE hFile = NULL;
		HANDLE hFileMapping = NULL;
		LPVOID addr = NULL;
		DWORD memoryToAlloc = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
		const char* error = TakeMapView(hFile, hFileMapping, addr, fileName, memoryToAlloc);
		if (error != NULL)
		{
			strcpy_s(lastErrorMessage, strlen(error) + 1, error);
			return NULL;
		}

		ZeroMemory(addr, memoryToAlloc);

		HTHANDLE* htHandle = new(addr) HTHANDLE(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
		htHandle->hFile = hFile;
		htHandle->hFileMapping = hFileMapping;
		htHandle->addr = addr;
		htHandle->tableMemorySize = memoryToAlloc;

		InitParsedFileName(fileName);
		htHandle->InitMutex();
		htHandle->SetIntervalSnapOn();

		std::thread startIntervalSnap(StartIntervalSnap, htHandle, htHandle->isIntervalSnapOn);
		startIntervalSnap.detach();
		CreateDirectoryForSnaps(fileName);

		return htHandle;
	}

	HTHANDLE* Open(const char fileName[CHAR_MAX_LENGTH])
	{
		HANDLE hFile = NULL;
		HANDLE hFileMapping = NULL;
		LPVOID addr = NULL;
		const char* error = TakeMapView(hFile, hFileMapping, addr, fileName);
		if (error != NULL)
		{
			strcpy_s(lastErrorMessage, strlen(error) + 1, error);
			return NULL;
		}

		HTHANDLE* htHandle = (HTHANDLE*)addr;
		htHandle->hFile = hFile;
		htHandle->hFileMapping = hFileMapping;
		htHandle->addr = addr;

		htHandle->SetFileName(fileName);
		InitParsedFileName(fileName);
		htHandle->InitMutex();
		htHandle->SetIntervalSnapOn();

		std::thread startIntervalSnap(StartIntervalSnap, htHandle, htHandle->isIntervalSnapOn);
		startIntervalSnap.detach();
		CreateDirectoryForSnaps(fileName);

		return htHandle;
	}

	BOOL Snap(HTHANDLE* htHandle)
	{
		bool result = true;
		try
		{
			WaitForSingleObject(htHandle->hMutex, INFINITE);

			if (!htHandle->isTableChanged)
			{
				ReleaseMutex(htHandle->hMutex);
				return true;
			}

			if (!FlushViewOfFile(htHandle->addr, htHandle->tableMemorySize))
			{
				throw FLUSH_VIEW_ERROR;
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

		ReleaseMutex(htHandle->hMutex);
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

		if (!UnmapViewOfFile(htHandle->addr))
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

	char* GetLastError(HTHANDLE* htHandle)
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