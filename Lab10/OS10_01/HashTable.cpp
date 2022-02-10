#include <iostream>
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

	Element* FindElementAddr(HTHANDLE* htHandle, const char* key)
	{
		Element* elementAddr = NULL;
		if (htHandle->currentSize == 0)
		{
			return elementAddr;
		}

		int index = GetIndexViaHash(key, htHandle->capacity);
			int startIndex = index;
		do
		{
			elementAddr = htHandle->GetElementAddr(index);
			htHandle->CorrectElementPointers(elementAddr);
			if (*(int*)elementAddr == NULL || !elementAddr->isDeleted && strcmp(elementAddr->getKey(), key) == 0)
			{
				return elementAddr;
			}

			NextIndex(index, startIndex, htHandle->capacity);
		} while (index != -1);

		return NULL;
	}

	Element* FindUnallocatedElementAddr(HTHANDLE* htHandle, const char* key)
	{
		Element* elementAddr = NULL;
		if (htHandle->currentSize == htHandle->capacity)
		{
			return elementAddr;
		}
		
		int index = GetIndexViaHash(key, htHandle->capacity);
		do
		{
			elementAddr = htHandle->GetElementAddr(index);
			htHandle->CorrectElementPointers(elementAddr);
			NextIndex(index, htHandle->capacity);
		} while (*(int*)elementAddr != NULL && !elementAddr->isDeleted && strcmp(elementAddr->getKey(), key) != 0);

		if (*(int*)elementAddr == NULL || elementAddr->isDeleted)
		{
			return elementAddr;
		}
		return NULL;
	}
}