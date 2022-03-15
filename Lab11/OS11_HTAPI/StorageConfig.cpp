#include "pch.h"
#include "StorageConfig.h"
#include "Element.h"
#include "SharedMemory.h"

namespace Core
{
	StorageConfig::StorageConfig(const StorageConfig& storageConfig)
	{
		this->capacity = storageConfig.capacity;
		this->maxKeyLength = storageConfig.maxKeyLength;
		this->maxPayloadLength = storageConfig.maxPayloadLength;
		this->secSnapshotInterval = storageConfig.secSnapshotInterval;

		this->storageMemorySize = storageConfig.storageMemorySize;
		this->elementMemorySize = storageConfig.elementMemorySize;
	}

	StorageConfig::StorageConfig(int capacity,
		int secSnapshotInterval,
		int maxKeyLength,
		int maxPayloadLength)
	{
		this->capacity = capacity;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		this->secSnapshotInterval = secSnapshotInterval;

		this->storageMemorySize = CalcStorageMaxSizeMemory();
		this->elementMemorySize = CalcElementMaxSizeMemory();
	}

	DWORD StorageConfig::CalcStorageMaxSizeMemory()
	{
		DWORD elementSize = CalcElementMaxSizeMemory();
		return sizeof(StorageConfig) + sizeof(SharedMemory) + this->capacity * (elementSize);
	}

	DWORD StorageConfig::CalcElementMaxSizeMemory()
	{
		return sizeof(Element) + this->maxKeyLength + this->maxPayloadLength;
	}

	int StorageConfig::getCapacity() const
	{
		return capacity;
	}

	int StorageConfig::getMaxKeyLength() const
	{
		return maxKeyLength;
	}

	int StorageConfig::getMaxPayloadLength() const
	{
		return maxPayloadLength;
	}

	int StorageConfig::getSecSnapshotInterval() const
	{
		return secSnapshotInterval;
	}

	DWORD StorageConfig::getStorageMemorySize() const
	{
		return storageMemorySize;
	}

	DWORD StorageConfig::getElementMemorySize() const
	{
		return elementMemorySize;
	}
}