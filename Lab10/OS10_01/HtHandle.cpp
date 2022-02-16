#include <iostream>
#include <thread>
#include <ctime>
#include "HtHandle.h"
#include "Helper.h"
#include "Api.h"

namespace HT
{
	SharedMemory::SharedMemory(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength)
	{
		this->currentSize = 0;
		this->isChangedFromLastSnap = false;

		this->capacity = capacity;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		this->secSnapshotInterval = secSnapshotInterval;
		this->tableMemorySize = CalcHashTableMaxSizeMemory(capacity, maxKeyLength, maxPayloadLength);
		this->elementMemorySize = CalcElementMaxSizeMemory(maxKeyLength, maxPayloadLength);
	}

	ParsedFileName::ParsedFileName() { }

	ParsedFileName::ParsedFileName(const char fileName[CHAR_MAX_LENGTH])
	{
		this->fileName = GetFileName(fileName);
		this->filePath = GetFilePath(fileName);
	}

	HTHANDLE::HTHANDLE()
	{
		InitDefault();
	}

	HTHANDLE::HTHANDLE(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		InitDefault();

		this->capacity = capacity;
		this->secSnapshotInterval = secSnapshotInterval;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		SetFileName(fileName);
	}

#pragma region Public Methods

	void HTHANDLE::SetLastError(const char error[CHAR_MAX_LENGTH])
	{
		strcpy_s(this->lastErrorMessage, strlen(error) + 1, error);
	}

	Element* HTHANDLE::GetElement(int index)
	{
		Element* element = GetElementAddr(index);
		CorrectElementPointers(element);
		return element;
	}

	std::string HTHANDLE::GenerateSnapFilename()
	{
		std::string snapFilename = parsedFileName.filePath + "/";
		snapFilename += SNAPSHOT_DIRECTORY_NAME;
		snapFilename += "/" + parsedFileName.fileName;
		snapFilename += "-" + std::to_string(currentSnap++);
		snapFilename += "." + TimeToLocalString(time(NULL));
		snapFilename += ".ht";

		return snapFilename;
	}

	void HTHANDLE::FinishIntervalSnap()
	{
		isIntervalSnapOn.store(false, std::memory_order_seq_cst);
	}

	void HTHANDLE::LaunchIntervalSnap()
	{
		CreateDirectoryForSnaps();

		std::thread startIntervalSnap(StartIntervalSnap, this);
		startIntervalSnap.detach();
	}

	void HTHANDLE::CreateHtFile()
	{
		hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			SetLastError(CREATE_FILE_ERROR);
			throw std::exception();
		}
	}

	void HTHANDLE::OpenHtFile()
	{
		hFile = CreateFileA(
			fileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			hFile = NULL;
		}
	}

	void HTHANDLE::CreateHtFileMapping(DWORD memoryToAlloc)
	{
		hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, fileName);
		if (hFileMapping == NULL)
		{
			SetLastError(CREATE_FILE_MAPING_ERROR);
			throw std::exception();
		}
	}

	void HTHANDLE::OpenHtFileMapping()
	{
		bool isFileNotOpened = hFile == NULL;
		DWORD memoryToAlloc = 0;
		if (isFileNotOpened)
		{
			memoryToAlloc = ReceiveHtMemorySizeFromSharedMemory();
		}

		hFileMapping = CreateFileMappingA(isFileNotOpened ? INVALID_HANDLE_VALUE : hFile, NULL, PAGE_READWRITE, 0, memoryToAlloc, fileName);
		if (hFileMapping == NULL)
		{
			SetLastError(OPEN_FILE_MAPPING_ERROR);
			throw std::exception();
		}
	}


	void HTHANDLE::CreateViewOfHtFile(DWORD memoryToAlloc)
	{
		addrStart = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addrStart == NULL)
		{
			SetLastError(CREATE_FILE_VIEW_ERROR);
			throw std::exception();
		}

		ZeroMemory(addrStart, memoryToAlloc);
	}

	void HT::HTHANDLE::OpenViewOfHtFile()
	{
		addrStart = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addrStart == NULL)
		{
			SetLastError(OPEN_FILE_VIEW_ERROR);
			throw std::exception();
		}
	}

	void HTHANDLE::CreateSharedMemory()
	{
		sharedMemory = new(addrStart) SharedMemory(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength);
	}

	void HT::HTHANDLE::OpenSharedMemory()
	{
		sharedMemory = (SharedMemory*)addrStart;
		CorrectHashTableInfo();
	}

#pragma endregion


#pragma region Private Methods

	void HTHANDLE::InitDefault()
	{
		capacity = 1;
		secSnapshotInterval = 3;
		maxKeyLength = 5;
		maxPayloadLength = 10;

		*lastErrorMessage = NULL;
		SetFileName(DEFAULT_FILE_NAME);
		intervalSnapMutexName = fileName; 
		intervalSnapMutexName += "-mutex";

		isIntervalSnapOn = true;
		isTableChangedFromLastSnap = false;
		currentSnap = 0;
		snapLastTime = NULL;

		hMutex = CreateMutexA(NULL, FALSE, fileName);
		hIntervalSnapMutex = CreateMutexA(NULL, FALSE, intervalSnapMutexName.c_str());
		hFile = NULL;
		hFileMapping = NULL;
		addrStart = NULL;
		sharedMemory = NULL;
		addrElementsStart = NULL;
	}

	void HTHANDLE::SetFileName(const char fileName[CHAR_MAX_LENGTH])
	{
		strcpy_s(this->fileName, strlen(fileName) + 1, fileName);
		parsedFileName = ParsedFileName(fileName);
	}

	Element* HTHANDLE::GetElementAddr(int index)
	{
		return (Element*)((char*)addrElementsStart + sharedMemory->elementMemorySize * index);
	}

	void HTHANDLE::CorrectElementPointers(Element* element)
	{
		LPVOID keyAddr = (char*)element + sizeof(Element);
		LPVOID payloadAddr = (char*)keyAddr + sharedMemory->maxKeyLength;
		element->setKeyPointer(keyAddr, element->keyLength);
		element->setPayloadPointer(payloadAddr, element->payloadLength);
	}

	void HTHANDLE::CreateDirectoryForSnaps()
	{
		std::string directoryToCreate = parsedFileName.filePath + "/";
		directoryToCreate += SNAPSHOT_DIRECTORY_NAME;
		if (!CreateDirectoryA(directoryToCreate.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		{
			SetLastError(CREATE_SNAPS_DIRECTORY_ERROR);
			throw std::exception();
		}
	}

	void HTHANDLE::CorrectHashTableInfo()
	{
		capacity = sharedMemory->capacity;
		maxKeyLength = sharedMemory->maxKeyLength;
		maxPayloadLength = sharedMemory->maxPayloadLength;
	}

	DWORD HT::HTHANDLE::ReceiveHtMemorySizeFromSharedMemory()
	{
		HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), fileName);
		if (hFileMapping == NULL)
		{
			SetLastError(OPEN_FILE_MAPPING_ERROR);
			throw std::exception();
		}

		HANDLE addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (addr == NULL)
		{
			SetLastError(OPEN_FILE_VIEW_ERROR);
			throw std::exception();
		}

		DWORD memorySize = ((SharedMemory*)addr)->tableMemorySize;

		if (!UnmapViewOfFile(addr))
		{
			SetLastError(CLOSE_FILE_VIEW_ERROR);
			throw std::exception();
		}

		if (!CloseHandle(hFileMapping))
		{
			SetLastError(CLOSE_FILE_MAPPING_ERROR);
			throw std::exception();
		}

		return memorySize;
	}

#pragma endregion


	void HT::StartIntervalSnap(HTHANDLE* htHandle)
	{
		WaitForSingleObject(htHandle->hIntervalSnapMutex, INFINITE);

		if (htHandle->isIntervalSnapOn.load(std::memory_order_seq_cst))
		{
			do
			{
				std::this_thread::sleep_for(std::chrono::seconds(htHandle->secSnapshotInterval));
			} while (htHandle->isIntervalSnapOn.load(std::memory_order_seq_cst) && Snap(htHandle));
		}

		ReleaseMutex(htHandle->hIntervalSnapMutex);
		delete htHandle;
	}
}