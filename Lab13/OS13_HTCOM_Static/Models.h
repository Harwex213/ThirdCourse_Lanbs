#pragma once

struct Element
{
public: // Constructors
	Element();
	Element(const void* key, int keyLength);
	Element(const void* key, int keyLength, const void* payload, int payloadLength);
	Element(const Element* oldElement, const void* payload, int payloadLength);

private: // Fields
	int keyLength;
	int payloadLength;
	const void* key;
	const void* payload;
	bool isDeleted;

public: // Getters & Setters
	void setKeyPointer(const void* key, int keyLength);
	bool setKey(const void* key, int keyLength);
	char* getKey() const;
	int getKeyLength() const;

	void setPayloadPointer(const void* payload, int payloadLength);
	bool setPayload(const void* payload, int payloadLength);
	char* getPayload() const;
	int getPayloadLength() const;

	bool getIsDeleted() const;
	void setIsDeleted(bool value);

private: // Private methods
	void initDefault();

public: // Public methods

};


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

