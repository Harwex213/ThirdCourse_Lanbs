#pragma once
#include <iostream>
#include <Windows.h>
#include "Element.h"

#define CHAR_MAX_LENGTH 512
#define CHAR_MAX_LENGTH_2 32
#define SNAPSHOT_DIRECTORY_NAME "snapshots"
#define SNAPSHOT_TIMER_NAME "snap-timer"
#define SECOND 10000000

namespace HT
{
	struct HTHANDLE
	{
		struct ParsedFileName
		{
			ParsedFileName(const char fileName[CHAR_MAX_LENGTH]);

			std::string fileName;
			std::string filePath;
		};

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
		ParsedFileName* parsedFileName;

		void initDefault();
		Element* GetElementAddr(int index);
		void CorrectElementPointers(LPVOID elementAddr);

		void SetFileName(const char fileName[CHAR_MAX_LENGTH]);
		void InitParsedFileName(const char fileName[CHAR_MAX_LENGTH]);
		void ClearParsedFileName();

		std::string GenerateSnapFilename();
		void SetIntervalSnapOn();
		void SetIntervalSnapOff();

		void InitMutex();
	};
}