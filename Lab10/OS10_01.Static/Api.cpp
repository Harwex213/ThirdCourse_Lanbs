#include "pch.h"
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
		HTHANDLE* htHandle = new HTHANDLE(fileName);

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
		if (htHandle == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return false;
		}

		bool result = true;
		try
		{
			WaitForSingleObject(htHandle->hMutex, INFINITE);

			if (!htHandle->sharedMemory->isChangedFromLastSnap)
			{
				ReleaseMutex(htHandle->hMutex);
				return true;
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
				htHandle->SetLastError(CREATE_SNAPFILE_ERROR);
				throw std::exception();
			}
			
			if (!WriteFile(hSnapFile, htHandle->addrStart, htHandle->sharedMemory->tableMemorySize, NULL, NULL))
			{
				htHandle->SetLastError(WRITE_SNAPFILE_ERROR);
				throw std::exception();
			}

			if (!CloseHandle(hSnapFile))
			{
				htHandle->SetLastError(CLOSE_SNAPFILE_ERROR);
				throw std::exception();
			};

			time(&htHandle->snapLastTime);
			htHandle->sharedMemory->isChangedFromLastSnap = false;
		}
		catch (std::exception&)
		{
			result = false;
		}

		ReleaseMutex(htHandle->hMutex);

		return result;
	}

	BOOL Close(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return false;
		}

		try
		{
			htHandle->FinishIntervalSnap();

			if (!Snap(htHandle))
			{
				return false;
			}

			htHandle->FlushHashTableData();
			htHandle->CloseViewOfHtFile();
			htHandle->CloseHtFileMapping();
			htHandle->CloseHtFile();
		}
		catch (const std::exception&)
		{
			return false;
		}

		delete htHandle;
		return true;
	}

	BOOL Insert(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL || element == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return false;
		}

		int keyLength = element->keyLength;
		int payloadLength = element->payloadLength;
		std::string key = TruncateStrByMax(keyLength, element->getKey(), htHandle->maxKeyLength);
		std::string payload = TruncateStrByMax(payloadLength, element->getPayload(), htHandle->maxPayloadLength);

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
		elementToInsert->setPayloadPointer(payloadAddr, payloadLength);
		elementToInsert->setKey(key.c_str(), keyLength);
		elementToInsert->setPayload(payload.c_str(), payloadLength);

		htHandle->sharedMemory->currentSize++;
		htHandle->sharedMemory->isChangedFromLastSnap = true;

		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	BOOL Update(HTHANDLE* htHandle, const Element* oldElement, const void* newPayload, int newPayloadlength)
	{
		if (htHandle == NULL || oldElement == NULL || newPayload == NULL || newPayloadlength == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return false;
		}

		int keyLength = oldElement->keyLength;
		int payloadLength = newPayloadlength;
		std::string key = TruncateStrByMax(keyLength, oldElement->getKey(), htHandle->maxKeyLength);
		std::string payload = TruncateStrByMax(payloadLength, (const char*)newPayload, htHandle->maxPayloadLength);

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		Element* elementToUpdate = FindElementAddr(htHandle, key.c_str());
		if (elementToUpdate == NULL)
		{
			htHandle->SetLastError(UPDATE_ERROR);
			ReleaseMutex(htHandle->hMutex);
			return false;
		}

		elementToUpdate->setPayload(payload.c_str(), payloadLength);

		htHandle->sharedMemory->isChangedFromLastSnap = true;
		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	BOOL Delete(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL || element == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return false;
		}

		int keyLength = element->keyLength;
		std::string key = TruncateStrByMax(keyLength, element->getKey(), htHandle->maxKeyLength);

		WaitForSingleObject(htHandle->hMutex, INFINITE);
		Element* elementToDelete = FindElementAddr(htHandle, key.c_str());
		if (elementToDelete == NULL)
		{
			htHandle->SetLastError(DELETE_ERROR);
			ReleaseMutex(htHandle->hMutex);
			return false;
		}

		elementToDelete->isDeleted = true;

		htHandle->sharedMemory->currentSize--;
		htHandle->sharedMemory->isChangedFromLastSnap = true;
		ReleaseMutex(htHandle->hMutex);
		return true;
	}

	Element* Get(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL || element == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return NULL;
		}

		int keyLength = element->keyLength;
		std::string key = TruncateStrByMax(keyLength, element->getKey(), htHandle->maxKeyLength);

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
		if (htHandle == NULL)
		{
			htHandle->SetLastError(ARGUMENT_NULL_ERROR);
			return;
		}

		for (int i = 0; i < htHandle->capacity; i++)
		{
			Element* elementAddr = htHandle->GetElement(i);
			if (*(int*)elementAddr == NULL || elementAddr->isDeleted)
			{
				Print(NULL);
				continue;
			}
			Print(elementAddr);
		}
	}
}