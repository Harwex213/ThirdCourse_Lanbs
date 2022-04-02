#include "pch.h"
#include "SharedMemory.h"

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
	return isChangedFromLastSnap.load(std::memory_order_seq_cst);
}

void SharedMemory::setIsChangedFromLastSnap(bool value)
{
	isChangedFromLastSnap.store(value, std::memory_order_seq_cst);
}