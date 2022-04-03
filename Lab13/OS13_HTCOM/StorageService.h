#pragma once
#include "pch.h"
#include "SharedMemory.h"
#include "StorageConfig.h"
#include "Element.h"
#include "HashTableService.h"

#define STORAGE_ALREADY_RECEIVED_ERROR "Storage memory already received."
#define STORAGE_CLEAR_ERROR "Storage memory not received."
#define ELEMENT_NOUT_FOUND_ERROR "Element not found in storage."
#define STORAGE_IS_FULL_ERROR "Storage is full already."
#define ELEMENT_KEY_ALREADY_EXIST "Element with such key already exist in storage."

struct StorageService
{
public: // Constructors
	StorageService();

private: // Fields
	HashTableService hashTableService;

	LPVOID storageMemoryStart;
	StorageConfig* storageConfig;
	SharedMemory* sharedMemory;
	LPVOID elementsMemoryStart;
	bool isMemoryReceived;

public: // Getters; Setters;
	LPVOID getStorageMemoryStart();
	StorageConfig* getStorageConfig();
	SharedMemory* getSharedMemory();
	void setIsMemoryReceived(bool value);
	bool getIsMemoryReceived();

private: // Private methods
	Element* getElementAddr(int index);
	void correctElementPointers(Element* elementAddr);
	Element* findUnallocatedMemory(const char* key);

public: // Public methods
	void InitializeStorage(LPVOID storageMemoryStart, const StorageConfig& storageConfig);
	void ReceiveStorage(LPVOID storageMemoryStart);
	void ClearStorage();
	Element* getElement(int index);
	Element* findElement(const char* key, int keyLength);
	void insertElement(const Element* element);
	void updateElement(const Element* element);
	void deleteElement(const char* key, int keyLength);

};
