#pragma once

struct StorageConfig
{
public: // Constructors
	StorageConfig(int capacity,
		int secSnapshotInterval,
		int maxKeyLength,
		int maxPayloadLength);
	StorageConfig(const StorageConfig& storageConfig);

public: // Fields
	int capacity;
	int maxKeyLength;
	int maxPayloadLength;
	int secSnapshotInterval;

	DWORD elementMemorySize;
	DWORD storageMemorySize;

public: // Getters; Setters;
	int getCapacity() const;
	int getMaxKeyLength() const;
	int getMaxPayloadLength() const;
	int getSecSnapshotInterval() const;
	DWORD getStorageMemorySize() const;
	DWORD getElementMemorySize() const;

private: // Private methods
	DWORD CalcStorageMaxSizeMemory();
	DWORD CalcElementMaxSizeMemory();

public: // Public methods

};