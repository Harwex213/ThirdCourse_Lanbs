#pragma once
#include "pch.h"
#include "SharedMemory.h"
#include "StorageConfig.h"
#include "Element.h"

struct StorageService
{
public: // Constructors
	StorageService();

private: // Fields
	LPVOID storageMemoryStart;
	StorageConfig* storageConfig;
	SharedMemory* sharedMemory;
	LPVOID elementsMemoryStart;

public: // Getters; Setters;
	LPVOID getStorageMemoryStart();
	StorageConfig* getStorageConfig();
	SharedMemory* getSharedMemory();

private: // Private methods
	Element* getElementAddr(int index);
	void correctElementPointers(Element* elementAddr);
	Element* findUnallocatedMemory(const char* key);

public: // Public methods
	void InitializeStorage(LPVOID storageMemoryStart, const StorageConfig& storageConfig);
	void ReceiveStorage(LPVOID storageMemoryStart);
	void ClearStorage();
	Element* findElement(const char* key, int keyLength);
	void insertElement(const Element* element);
	void updateElement(const Element* element);
	void deleteElement(const char* key, int keyLength);

};
