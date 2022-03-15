#include "pch.h"
#include "HtHandle.h"
#include "StorageConfig.h"
#include "Error.h"

namespace Core
{
	HTHANDLE::HTHANDLE(const char fileName[CHAR_MAX_LENGTH])
	{
		strcpy_s(this->fileName, strlen(fileName) + 1, fileName);
		*this->lastErrorMessage = NULL;

		std::string mutexName = fileName; mutexName += "-mutex";
		strcpy_s(this->mutexName, mutexName.size() + 1, mutexName.c_str());
		this->hMutex = CreateMutexA(NULL, FALSE, this->mutexName);
		if (this->hMutex == INVALID_MUTEX_HANDLE)
		{
			throw std::exception(INVALID_MUTEX_HANDLE_ERROR);
		}
		this->hFile = NULL;
		this->hFileMapping = NULL;
		this->snapService = NULL;
		this->hAliveEvent = NULL;
		this->state = HTHANDLE::State::IDLE;
	}

	void HTHANDLE::setLastErrorMessage(const char* error)
	{
		this->isInitError = true;
		strcpy_s(this->lastErrorMessage, strlen(error) + 1, error);
	}

	char* HTHANDLE::getLastErrorMessage()
	{
		return this->lastErrorMessage;
	}

	HTHANDLE::State HTHANDLE::getState()
	{
		return this->state.load(std::memory_order_seq_cst);
	}

	void HTHANDLE::setState(State state)
	{
		this->state.store(state, std::memory_order_seq_cst);
	}

	HANDLE HTHANDLE::getAliveEventHandle()
	{
		return this->hAliveEvent;
	}

	void HTHANDLE::setIsInitError(bool value)
	{
		this->isInitError = value;
	}

	bool HTHANDLE::getIsInitError()
	{
		return this->isInitError;
	}

	StorageConfig* HTHANDLE::getStorageConfig()
	{
		return storageService.getStorageConfig();
	}

	void HTHANDLE::CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength)
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() != HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_IDLE_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			this->hFile = fileService.createFile(this->fileName);
			if (this->hFile == INVALID_FILE_HANDLE)
			{
				this->hFile = NULL;
				throw std::exception(CREATE_FILE_ERROR);
			}

			StorageConfig storageConfig = StorageConfig(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength);
			this->hFileMapping = fileMappingService.createFileMapping(this->hFile, storageConfig.getStorageMemorySize(), this->fileName);
			if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
			{
				this->hFileMapping = NULL;
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			LPVOID addr = fileViewService.mapFileView(this->hFileMapping);
			if (addr == INVALID_ADDR_FILE_VIEW)
			{
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			storageService.InitializeStorage(addr, storageConfig);

			if (fileViewService.unmapFileView(this->storageService.getStorageMemoryStart()) == FALSE)
			{
				throw std::exception(UNMAP_FILE_VIEW_ERROR);
			}

			if (fileMappingService.closeFileMapping(this->hFileMapping) == FALSE)
			{
				throw std::exception(CLOSE_FILE_MAPPING_ERROR);
			}

			if (fileService.closeFile(this->hFile) == FALSE)
			{
				throw std::exception(CLOSE_FILE_ERROR);
			}
		}
		catch (const std::exception& error)
		{
			if (this->hFile != NULL)
			{
				fileService.closeFile(this->hFile);
			}
			if (this->hFileMapping != NULL)
			{
				fileMappingService.closeFileMapping(this->hFileMapping);
			}
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::OpenStorage()
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() != HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_IDLE_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			this->hFileMapping = fileMappingService.openFileMapping(this->fileName);
			if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
			{
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			LPVOID addr = fileViewService.mapFileView(this->hFileMapping);
			if (addr == INVALID_ADDR_FILE_VIEW)
			{
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			storageService.ReceiveStorage(addr);

			this->setState(HTHANDLE::State::OPEN);

			this->startAliveEventReceiver();
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::LoadStorage()
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() != HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_IDLE_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			this->hFile = fileService.openFile(this->fileName);
			if (this->hFile == INVALID_FILE_HANDLE)
			{
				throw std::exception(CREATE_FILE_ERROR);
			}

			DWORD memoryToAlloc = ALLOC_ALL_MEMORY_VALUE;			
			this->hFileMapping = fileMappingService.createFileMapping(this->hFile, memoryToAlloc, this->fileName);
			if (this->hFileMapping == INVALID_FILE_MAPPING_HANDLE)
			{
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			LPVOID addr = fileViewService.mapFileView(this->hFileMapping);
			if (addr == INVALID_ADDR_FILE_VIEW)
			{
				throw std::exception(CREATE_FILE_MAPING_ERROR);
			}

			storageService.ReceiveStorage(addr);

			StorageConfig* config = this->storageService.getStorageConfig();
			this->snapService = new SnapService(this->fileName, this->storageService.getStorageMemoryStart(), config->getStorageMemorySize());
			this->intervalSnapService.startIntervalSnap(this->snapService, config->getSecSnapshotInterval());

			this->setState(HTHANDLE::State::LOAD);

			this->startAliveEventEmitter();
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::executeSnapInternal()
	{
		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			if (storageService.getSharedMemory()->getIsChangedFromLastSnap() == false)
			{
				ReleaseMutex(this->hMutex);
				return;
			}
			snapService->executeSnap();
			storageService.getSharedMemory()->setIsChangedFromLastSnap(false);
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::ExecuteSnap()
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() != HTHANDLE::State::LOAD)
		{
			throw std::exception(NOT_SNAPSHOT_OWNER_ERROR);
		}

		this->executeSnapInternal();
	}

	void HTHANDLE::CloseStorage()
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}

		intervalSnapService.setIsIntervalSnapOn(false);

		if (snapService != NULL)
		{
			this->executeSnapInternal();
		}

		bool isFlushed = fileViewService.flushFileViewToDrive(this->storageService.getStorageMemoryStart(),
			this->storageService.getStorageConfig()->getStorageMemorySize());
		if (isFlushed == false)
		{
			throw std::exception(FLUSH_VIEW_ERROR);
		}

		if (fileViewService.unmapFileView(this->storageService.getStorageMemoryStart()) == FALSE)
		{
			throw std::exception(UNMAP_FILE_VIEW_ERROR);
		}

		if (fileMappingService.closeFileMapping(this->hFileMapping) == FALSE)
		{
			throw std::exception(CLOSE_FILE_MAPPING_ERROR);
		}

		if (this->hFile != NULL && fileService.closeFile(this->hFile) == FALSE)
		{
			throw std::exception(CLOSE_FILE_ERROR);
		}

		if (CloseHandle(this->hAliveEvent) == FALSE)
		{
			throw std::exception(CLOSE_EVENT_ERROR);
		}

		this->hFile = NULL;
		this->hFileMapping = NULL;
		this->snapService = NULL;
		this->hAliveEvent = NULL;
		this->storageService.ClearStorage();
		this->setState(HTHANDLE::State::IDLE);
	}

	void HTHANDLE::Insert(const Element* element)
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}
		if (element == NULL)
		{
			throw std::exception(ELEMENT_ARGUMENT_NULL_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			storageService.insertElement(element);
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::Update(const Element* oldElement, const void* newPayload, int newPayloadLength)
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}
		if (oldElement == NULL)
		{
			throw std::exception(ELEMENT_ARGUMENT_NULL_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			storageService.updateElement(new Element(oldElement, newPayload, newPayloadLength));
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	void HTHANDLE::Delete(const Element* element)
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}
		if (element == NULL)
		{
			throw std::exception(ELEMENT_ARGUMENT_NULL_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		try
		{
			storageService.deleteElement(element->getKey(), element->getKeyLength());
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);
	}

	Element* HTHANDLE::Get(const Element* element)
	{
		std::lock_guard<std::mutex> lock(this->internalMutex);

		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}
		if (element == NULL)
		{
			throw std::exception(ELEMENT_ARGUMENT_NULL_ERROR);
		}

		WaitForSingleObject(this->hMutex, INFINITE);

		Element* foundElement = NULL;
		try
		{
			foundElement = storageService.findElement(element->getKey(), element->getKeyLength());
			if (foundElement == ELEMENT_NOT_FOUND)
			{
				throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
			}
		}
		catch (const std::exception& error)
		{
			ReleaseMutex(this->hMutex);
			throw error;
		}

		ReleaseMutex(this->hMutex);

		return foundElement;
	}

	void HTHANDLE::PrintElement(const Element* element)
	{
		if (element == NULL)
		{
			printf_s("Printing elemenet. NULL\n");
			return;
		}
		printf_s("Printing element. %s: %s\n", element->getKey(), element->getPayload());
	}


	void HTHANDLE::PrintAllElements()
	{
		if (this->getState() == HTHANDLE::State::IDLE)
		{
			throw std::exception(STORAGE_NOT_OPENED_ERROR);
		}

		for (size_t i = 0; i < storageService.getStorageConfig()->getCapacity(); i++)
		{
			Element* element = storageService.getElement(i);
			PrintElement(element);
		}
	}

	void HTHANDLE::startAliveEventEmitter()
	{
		std::string eventName = this->fileName;
		eventName += "-event";
		this->hAliveEvent = CreateEventA(NULL, TRUE, TRUE, eventName.c_str());
		if (this->hAliveEvent == NULL)
		{
			throw std::exception(CREATE_EVENT_ERROR);
		}

		std::thread eventReceiver(startNotifyingAliveEvent, this);
		eventReceiver.detach();
	}

	void startNotifyingAliveEvent(HTHANDLE* htHandle)
	{
		HANDLE hEvent = htHandle->getAliveEventHandle();
		printf_s("started event\n");
		while (htHandle->getState() != HTHANDLE::IDLE)
		{
			ResetEvent(hEvent);
			Sleep(50);
			SetEvent(hEvent);
		}

		printf_s("finished event\n");
		ResetEvent(hEvent);
	}

	void HTHANDLE::startAliveEventReceiver()
	{
		std::string eventName = this->fileName;
		eventName += "-event";
		this->hAliveEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, eventName.c_str());
		if (this->hAliveEvent == NULL)
		{
			throw std::exception(CREATE_EVENT_ERROR);
		}

		std::thread eventReceiver(startListeningAliveEvent, this);
		eventReceiver.detach();
		Sleep(180);
	}



	void startListeningAliveEvent(HTHANDLE* htHandle)
	{
		HANDLE hEvent = htHandle->getAliveEventHandle();
		DWORD result = 0;
		do
		{
			result = WaitForSingleObject(hEvent, 100);
			Sleep(100);
		} while (result == WAIT_OBJECT_0 && htHandle->getState() != HTHANDLE::IDLE);

		printf_s("Connection with START has been lost. Trying to close storage\n");

		try
		{
			htHandle->CloseStorage();
		}
		catch (const std::exception& error)
		{
			printf_s("%s. GetLastError: %d\n", error.what(), GetLastError());
		}
	}
}