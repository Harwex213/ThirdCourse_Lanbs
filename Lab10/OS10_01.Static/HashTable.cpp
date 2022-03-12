#include "pch.h"
#include "HashTable.h"
#include "Element.h"

namespace HT
{
	int GetIndexViaHash(const char* key, const int& capacity)
	{
		return std::hash<std::string_view>()(std::string_view(key, std::strlen(key))) % capacity;
	}

	void NextIndex(int& index, const int& capacity)
	{
		index++;
		index %= capacity;
	}

	void NextIndex(int& index, int startIndex, const int& capacity)
	{
		index++;
		index %= capacity;

		if (index == startIndex)
			index = -1;
	}

	// Returns pointer if found el with set key, otherwise NULL;
	Element* FindElementAddr(HTHANDLE* htHandle, const char* key)
	{
		Element* elementAddr;
		if (htHandle->sharedMemory->currentSize == 0)
		{
			return NULL;
		}

		int index = GetIndexViaHash(key, htHandle->capacity);
		int startIndex = index;
		do
		{
			elementAddr = htHandle->GetElement(index);
			if (*(int*)elementAddr == NULL)
			{
				return NULL;
			}
			if (!elementAddr->isDeleted && strcmp(elementAddr->getKey(), key) == 0)
			{
				return elementAddr;
			}

			NextIndex(index, startIndex, htHandle->capacity);
		} while (index != -1);

		return NULL;
	}

	// Returns pointer if found free space, otherwise NULL; On founding element with same key returns NULL;
	Element* FindUnallocatedElementAddr(HTHANDLE* htHandle, const char* key)
	{
		Element* elementAddr;
		if (htHandle->sharedMemory->currentSize == htHandle->capacity)
		{
			return NULL;
		}
		
		int index = GetIndexViaHash(key, htHandle->capacity);
		do
		{
			elementAddr = htHandle->GetElement(index);
			NextIndex(index, htHandle->capacity);
		} while (*(int*)elementAddr != NULL && !elementAddr->isDeleted && strcmp(elementAddr->getKey(), key) != 0);

		if (*(int*)elementAddr == NULL || elementAddr->isDeleted)
		{
			return elementAddr;
		}
		return NULL;
	}
}