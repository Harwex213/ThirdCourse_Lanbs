#pragma once
#include "pch.h"
#include "Element.h"
#include "SharedMemory.h"
#include "StorageConfig.h"
#include "HashTableService.h"

#define ELEMENT_NOT_FOUND NULL

namespace Core
{
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
		Element* getElement(int index);
		Element* findElement(const char* key, int keyLength);
		void insertElement(const Element* element);
		void updateElement(const Element* element);
		void deleteElement(const char* key, int keyLength);
	};
}