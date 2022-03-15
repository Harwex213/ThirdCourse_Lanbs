#include "pch.h"
#include "SharedMemory.h"

namespace Core
{
	SharedMemory::SharedMemory()
	{
		this->currentSize = 0;
		this->isChangedFromLastSnap = false;
	}

	int SharedMemory::getCurrentSize()
	{
		return currentSize;
	}
	
	void SharedMemory::incCurrentSize()
	{
		currentSize++;
	}

	void SharedMemory::decCurrentSize()
	{
		if (currentSize == 0)
		{
			return;
		}

		currentSize--;
	}

	bool SharedMemory::getIsChangedFromLastSnap()
	{
		return isChangedFromLastSnap;
	}
	
	void SharedMemory::setIsChangedFromLastSnap(bool value)
	{
		isChangedFromLastSnap = value;
	}
}