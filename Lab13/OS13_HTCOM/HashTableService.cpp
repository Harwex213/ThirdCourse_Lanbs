#include "pch.h"
#include "HashTableService.h"

int HashTableService::getIndexViaHash(const char* key, const int& storageCapacity)
{
	return std::hash<std::string_view>()(std::string_view(key, std::strlen(key))) % storageCapacity;
}

int HashTableService::getNextIndex(int index, const int& capacity)
{
	index++;
	index %= capacity;

	return index;

}

int HashTableService::getNextIndex(int index, int initialIndex, const int& capacity)
{
	index++;
	index %= capacity;

	if (index == initialIndex)
		return -1;
	return index;
}
