#pragma once

namespace Core
{
	struct HashTableService
	{
		int getIndexViaHash(const char* key, const int& storageCapacity);
		int getNextIndex(int index, const int& capacity);
		int getNextIndex(int index, int initialIndex, const int& capacity);
	};
}