#pragma once
#include <iostream>
#include <Windows.h>
#include "Element.h"

#define CHAR_MAX_LENGTH 512
#define CHAR_MAX_LENGTH_2 32
#define SNAPSHOT_DIRECTORY_NAME "snapshots"
#define SNAPSHOT_TIMER_NAME "snap-timer"
#define SECOND 10000000

#define CREATE_FILE_MAPING_ERROR "Cannot create File Mapping. Check your filename"
#define CREATE_MAP_VIEW_ERROR "Cannot Map view of file. Check your filename"

#define FLUSH_VIEW_ERROR "Cannot flush view of file. Check your file"
#define CREATE_SNAPFILE_ERROR "Cannot create snapshot file"
#define WRITE_SNAPFILE_ERROR "Cannot write to snapshot file"
#define CLOSE_SNAPFILE_ERROR "Cannot close snapshot file"

#define UNMAP_VIEW_ERROR "Cannot unmap view of file"
#define CLOSE_FILE_MAPPING_ERROR "Cannot destroy file mapping"
#define CLOSE_FILE_ERROR "Cannot close file"

#define INSERT_ERROR "Cannot insert element. Table is full or such element already exist"
#define UPDATE_ERROR "Cannot find element for update"
#define DELETE_ERROR "Cannot find element for delete"
#define GET_ERROR "Cannot find element"

namespace HT
{
	struct ParsedFileName
	{
		ParsedFileName(const char fileName[CHAR_MAX_LENGTH]);

		std::string fileName;
		std::string filePath;
	};

	struct HTHANDLE
	{
		HTHANDLE();
		HTHANDLE(int capacity, int cecSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH]);

		int capacity;
		int secSnapshotInterval;
		int maxKeyLength;
		int maxPayloadLength;

		DWORD elementMemorySize;
		DWORD tableMemorySize;

		bool isTableChanged;

		int currentSize;
		DWORD currentSnap;
		time_t snapLastTime;
		std::atomic<bool>* isIntervalSnapOn;

		HANDLE hFile;
		HANDLE hFileMapping;
		HANDLE hMutex;
		LPVOID addr;

		char lastErrorMessage[CHAR_MAX_LENGTH];
		const char defaultFileName[12] = "./table.ht";
		char fileName[CHAR_MAX_LENGTH];

		void initDefault();
		Element* GetElementAddr(int index);
		void CorrectElementPointers(LPVOID elementAddr);

		void SetLastError(const char error[CHAR_MAX_LENGTH]);
		void SetFileName(const char fileName[CHAR_MAX_LENGTH]);

		std::string GenerateSnapFilename();
		void SetIntervalSnapOn();
		void SetIntervalSnapOff();

		void InitMutex();
	};

	static char lastErrorMessage[CHAR_MAX_LENGTH];
	static ParsedFileName* parsedFileName = NULL;

	void InitParsedFileName(const char fileName[CHAR_MAX_LENGTH]);
}