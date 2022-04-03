#include "pch.h"
#include "StorageService.h"
#include "Helper.h"

StorageService::StorageService()
{
	this->storageMemoryStart = NULL;
	this->storageConfig = NULL;
	this->sharedMemory = NULL;
	this->elementsMemoryStart = NULL;
	this->isMemoryReceived = false;
}

LPVOID StorageService::getStorageMemoryStart()
{
	return this->storageMemoryStart;
}

StorageConfig* StorageService::getStorageConfig()
{
	return this->storageConfig;
}

SharedMemory* StorageService::getSharedMemory()
{
	return this->sharedMemory;
}

void StorageService::setIsMemoryReceived(bool value)
{
	this->isMemoryReceived = value;
}

bool StorageService::getIsMemoryReceived()
{
	return this->isMemoryReceived;
}

Element* StorageService::getElementAddr(int index)
{
	return (Element*)((char*)elementsMemoryStart + storageConfig->getElementMemorySize() * index);
}

void StorageService::correctElementPointers(Element* element)
{
	LPVOID keyAddr = (char*)element + sizeof(Element);
	LPVOID payloadAddr = (char*)keyAddr + storageConfig->getMaxKeyLength();

	element->setKeyPointer(keyAddr, element->getKeyLength());
	element->setPayloadPointer(payloadAddr, element->getPayloadLength());
}

void StorageService::InitializeStorage(LPVOID storageMemoryStart, const StorageConfig& storageConfig)
{
	if (this->getIsMemoryReceived())
	{
		throw std::exception(STORAGE_ALREADY_RECEIVED_ERROR);
	}
	//ZeroMemory(this->storageMemoryStart, storageConfig.getStorageMemorySize());

	this->storageMemoryStart = storageMemoryStart;
	this->storageConfig = new(storageMemoryStart) StorageConfig(storageConfig);

	LPVOID sharedMemoryPointer = this->storageConfig + 1;
	this->sharedMemory = new (sharedMemoryPointer) SharedMemory();

	LPVOID elementsMemoryPointer = this->sharedMemory + 1;
	this->elementsMemoryStart = elementsMemoryPointer;

	this->setIsMemoryReceived(true);
}

void StorageService::ReceiveStorage(LPVOID storageMemoryStart)
{
	if (this->getIsMemoryReceived())
	{
		throw std::exception(STORAGE_ALREADY_RECEIVED_ERROR);
	}

	this->storageMemoryStart = storageMemoryStart;
	this->storageConfig = (StorageConfig*)storageMemoryStart;

	LPVOID sharedMemoryPointer = this->storageConfig + 1;
	this->sharedMemory = (SharedMemory*)sharedMemoryPointer;

	LPVOID elementsMemoryPointer = this->sharedMemory + 1;
	this->elementsMemoryStart = elementsMemoryPointer;

	this->setIsMemoryReceived(true);
}

void StorageService::ClearStorage()
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	storageMemoryStart = NULL;
	storageConfig = NULL;
	sharedMemory = NULL;
	elementsMemoryStart = NULL;

	this->setIsMemoryReceived(false);
}

Element* StorageService::getElement(int index)
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	Element* element = getElementAddr(index);
	if ((*(int*)element == NULL || element->getIsDeleted()))
	{
		return NULL;
	}
	else
	{
		correctElementPointers(element);
		return element;
	}
}

Element* StorageService::findElement(const char* pKey, int pKeyLength)
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	if (sharedMemory->getCurrentSize() == 0)
	{
		return NULL;
	}

	int keyLength = pKeyLength;
	std::string key = Helper::truncateString(pKey, keyLength, storageConfig->getMaxKeyLength());

	Element* element;
	int storageCapacity = storageConfig->getCapacity();
	int index = hashTableService.getIndexViaHash(key.c_str(), storageCapacity);
	int startIndex = index;
	do
	{
		element = getElementAddr(index);
		if (*(int*)element == NULL)
		{
			return NULL;
		}
		correctElementPointers(element);
		if (!element->getIsDeleted() && strcmp(element->getKey(), key.c_str()) == 0)
		{
			return element;
		}

		index = hashTableService.getNextIndex(index, startIndex, storageCapacity);
	} while (index != -1);

	return NULL;

}

Element* StorageService::findUnallocatedMemory(const char* key)
{
	int storageCapacity = storageConfig->getCapacity();
	if (sharedMemory->getCurrentSize() == storageCapacity)
	{
		throw std::exception(STORAGE_IS_FULL_ERROR);
	}

	Element* element;
	int index = hashTableService.getIndexViaHash(key, storageCapacity);
	do
	{
		element = getElementAddr(index);
		correctElementPointers(element);
		index = hashTableService.getNextIndex(index, storageCapacity);
	} while (*(int*)element != NULL && !element->getIsDeleted() && strcmp(element->getKey(), key) != 0);

	if (*(int*)element == NULL || element->getIsDeleted())
	{
		return element;
	}

	throw std::exception(ELEMENT_KEY_ALREADY_EXIST);
}

void StorageService::insertElement(const Element* element)
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	if (sharedMemory->getCurrentSize() == storageConfig->getCapacity())
	{
		throw std::exception(STORAGE_IS_FULL_ERROR);
	}

	int keyLength = element->getKeyLength();
	std::string key = Helper::truncateString(element->getKey(), keyLength, storageConfig->getMaxKeyLength());
	int payloadLength = element->getPayloadLength();
	std::string payload = Helper::truncateString(element->getPayload(), payloadLength, storageConfig->getMaxPayloadLength());

	Element* newElement = findUnallocatedMemory(key.c_str());
	LPVOID keyAddr = (char*)newElement + sizeof(Element);
	LPVOID payloadAddr = (char*)keyAddr + storageConfig->getMaxKeyLength();

	newElement->setKeyPointer(keyAddr, keyLength);
	newElement->setPayloadPointer(payloadAddr, payloadLength);
	newElement->setKey(key.c_str(), keyLength);
	newElement->setPayload(payload.c_str(), payloadLength);
	newElement->setIsDeleted(false);

	sharedMemory->incCurrentSize();
	sharedMemory->setIsChangedFromLastSnap(true);
}

void StorageService::updateElement(const Element* element)
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	if (sharedMemory->getCurrentSize() == 0)
	{
		throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
	}

	Element* elementToUpdate = findElement(element->getKey(), element->getKeyLength());
	if (elementToUpdate == NULL)
	{
		throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
	}

	int payloadLength = element->getPayloadLength();
	std::string payload = Helper::truncateString(element->getPayload(), payloadLength, storageConfig->getMaxPayloadLength());
	elementToUpdate->setPayload(payload.c_str(), payloadLength);

	sharedMemory->setIsChangedFromLastSnap(true);
}

void StorageService::deleteElement(const char* key, int keyLength)
{
	if (this->getIsMemoryReceived() == false)
	{
		throw std::exception(STORAGE_CLEAR_ERROR);
	}

	if (sharedMemory->getCurrentSize() == 0)
	{
		throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
	}

	Element* elementToDelete = findElement(key, keyLength);
	if (elementToDelete == NULL)
	{
		throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
	}

	elementToDelete->setIsDeleted(true);

	sharedMemory->decCurrentSize();
	sharedMemory->setIsChangedFromLastSnap(true);
}