#pragma once
#include "pch.h"
#include "SharedMemory.h"
#include "Element.h"
#include "StorageConfig.h"
#include "FileService.h"
#include "FileMappingService.h"
#include "FileViewService.h"
#include "HashTableService.h"
#include "IntervalSnapService.h"
#include "StorageService.h"
#include "StorageConfig.h"

#define INVALID_MUTEX_HANDLE NULL

#define CREATE_EVENT_ERROR "Cannot create event emittier for notifying alive events"
#define OPEN_EVENT_ERROR "Cannot open event receiver for listenning alive events from START"
#define CLOSE_EVENT_ERROR "Cannot close event"

#define STORAGE_NOT_IDLE_ERROR "Error occupied. Storage must be closed before"
#define STORAGE_NOT_OPENED_ERROR "Error occupied. Storage must be opened/loaded before"
#define NOT_SNAPSHOT_OWNER_ERROR "Cannot create snapshot file due to process is not owner of the file"

#define CHAR_MAX_LENGTH 512

namespace Core
{
	struct HTHANDLE
	{
		enum State
		{
			IDLE,
			LOAD,
			OPEN
		};

	public: // Constructors
		HTHANDLE(const char fileName[CHAR_MAX_LENGTH]);

	private: // Fields
		FileService fileService;
		FileMappingService fileMappingService;
		FileViewService fileViewService;
		StorageService storageService;
		SnapService* snapService;
		IntervalSnapService intervalSnapService;
		std::atomic<State> state;
		std::mutex internalMutex;

		HANDLE hMutex;
		HANDLE hFile;
		HANDLE hFileMapping;
		HANDLE hAliveEvent;
		char lastErrorMessage[CHAR_MAX_LENGTH];
		char fileName[CHAR_MAX_LENGTH];
		char mutexName[CHAR_MAX_LENGTH + 20];
	public:
		bool isInitError;

	public: // Getters; Setters;
		void setLastErrorMessage(const char* error);
		char* getLastErrorMessage();
		State getState();
		void setState(State state);
		HANDLE getAliveEventHandle();
		void setIsInitError(bool value);
		bool getIsInitError();
		StorageConfig* getStorageConfig();

	private: // Private methods
		void PrintElement(const Element* element);
		void startAliveEventEmitter();
		void startAliveEventReceiver();
		void executeSnapInternal();

	public: // Public methods
		void CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength);
		void OpenStorage();
		void LoadStorage();
		void ExecuteSnap();
		void CloseStorage();

		void Insert(const Element* element);
		void Update(const Element* oldElement, const void* newPayload, int newPayloadLength);
		void Delete(const Element* element);
		Element* Get(const Element* element);

		void PrintAllElements();
	};

	void startNotifyingAliveEvent(HTHANDLE* htHandle);
	void startListeningAliveEvent(HTHANDLE* htHandle);
}