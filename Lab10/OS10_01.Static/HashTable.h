#pragma once
#include "HtHandle.h"

namespace HT
{
	int GetIndexViaHash(const char* key, const int& capacity);
	void NextIndex(int& index, const int& capacity);
	void NextIndex(int& index, int startIndex, const int& capacity);
	Element* FindElementAddr(HTHANDLE* htHandle, const char* key);
	Element* FindUnallocatedElementAddr(HTHANDLE* htHandle, const char* key);
}