#pragma once
#include <iostream>
#include <Windows.h>
#include "Element.h"

#define CHAR_MAX_LENGTH 512
#define SNAPSHOT_DIRECTORY_NAME "snapshots"

#define DEFAULT_FILE_NAME "./table.ht"

#define CREATE_FILE_ERROR "Cannot create hash-table file for mapping. It can be due to wrong filename of another process already create HT"
#define CREATE_FILE_MAPING_ERROR "Cannot create File Mapping. Check your filename"
#define CREATE_FILE_VIEW_ERROR "Cannot create view of file. Check your filename"
#define CREATE_SNAPS_DIRECTORY_ERROR "Cannot create directory for snaps. Check your filename"

#define OPEN_FILE_MAPPING_ERROR "Cannot open file mapping"
#define OPEN_FILE_VIEW_ERROR "Cannot open view of file"

#define CLOSE_FILE_ERROR "Cannot close hash-table file"
#define CLOSE_FILE_MAPPING_ERROR "Cannot close file mapping"
#define CLOSE_FILE_VIEW_ERROR "Cannot close view of file"

#define NOT_FILE_OWNER_ERROR "Cannot create snapshot file due to process doesn't own the file"
#define CREATE_SNAPFILE_ERROR "Cannot create snapshot file"
#define WRITE_SNAPFILE_ERROR "Cannot write to snapshot file"
#define CLOSE_SNAPFILE_ERROR "Cannot close snapshot file"

#define FLUSH_VIEW_ERROR "Cannot flush view of file. Check your file"
#define UNMAP_VIEW_ERROR "Cannot unmap view of file"
#define CLOSE_FILE_MAPPING_ERROR "Cannot destroy file mapping"
#define CLOSE_FILE_ERROR "Cannot close file"

#define INSERT_ERROR "Cannot insert element. Table is full or such element already exist"
#define UPDATE_ERROR "Cannot find element for update"
#define DELETE_ERROR "Cannot find element for delete"
#define GET_ERROR "Cannot find element"

namespace HT
{
	struct SharedMemory
	{
		SharedMemory(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength);

		int currentSize;
		bool isChangedFromLastSnap;

		int capacity;
		int maxKeyLength;
		int maxPayloadLength;
		int secSnapshotInterval;
		DWORD elementMemorySize;
		DWORD tableMemorySize;
	};

	struct ParsedFileName
	{
		ParsedFileName();
		ParsedFileName(const char fileName[CHAR_MAX_LENGTH]);

		std::string fileName;
		std::string filePath;
	};

	struct HTHANDLE
	{
		HTHANDLE();
		HTHANDLE(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH]);

		int capacity;
		int maxKeyLength;
		int maxPayloadLength;
		int secSnapshotInterval;

		HANDLE hMutex;
		HANDLE hIntervalSnapMutex;
		HANDLE hFile;
		HANDLE hFileMapping;
		LPVOID addrStart;
		SharedMemory* sharedMemory;
		LPVOID addrElementsStart;

		char lastErrorMessage[CHAR_MAX_LENGTH];
		char fileName[CHAR_MAX_LENGTH];
		ParsedFileName parsedFileName;
		std::string intervalSnapMutexName;

		std::atomic<bool> isIntervalSnapOn;
		bool isTableChangedFromLastSnap;
		DWORD currentSnap;
		time_t snapLastTime;

		void SetLastError(const char error[CHAR_MAX_LENGTH]);
		Element* GetElement(int index);
		std::string GenerateSnapFilename();
		void FinishIntervalSnap();
		void LaunchIntervalSnap();
		void CreateHtFile();
		void OpenHtFile();
		void CreateHtFileMapping(DWORD memoryToAlloc);
		void OpenHtFileMapping();
		void CreateViewOfHtFile(DWORD memoryToAlloc);
		void OpenViewOfHtFile();
		void CreateSharedMemory();
		void OpenSharedMemory();

	private:
		void InitDefault();
		void SetFileName(const char error[CHAR_MAX_LENGTH]);
		Element* GetElementAddr(int index);
		void CorrectElementPointers(Element* elementAddr);
		void CreateDirectoryForSnaps();
		void CorrectHashTableInfo();
		DWORD ReceiveHtMemorySizeFromSharedMemory();
	};

	void StartIntervalSnap(HTHANDLE* htHandle);
}