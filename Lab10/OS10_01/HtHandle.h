#pragma once
#include <Windows.h>
#include "Element.h"

#define CHAR_MAX_LENGTH 512

namespace HT
{
	struct HTHANDLE
	{
		HTHANDLE();
		HTHANDLE(int capacity, int cecSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH]);

		const char defaultFileName[12] = "./input/txt";

		int capacity;					// емкость хранилища в количестве элементов 
		int secSnapshotInterval;		// переодичность сохранения в сек. 
		int maxKeyLength;				// максимальная длина ключа
		int maxPayloadLength;			// максимальная длина данных
		char fileName[CHAR_MAX_LENGTH];			    // имя файла 
		int elementSize;
		int currentSize;

		HANDLE hFile;					// File HANDLE != 0, если файл открыт
		HANDLE hFileMapping;			// Mapping File HANDLE != 0, если mapping создан  
		LPVOID addr;					// Addr != NULL, если mapview выполнен  

		time_t snapLastTime;			// дата последнего snap'a (time())  
		char lastErrorMessage[CHAR_MAX_LENGTH];		// сообщение об последней ошибке или 0x00

		void initDefault();
		Element* GetElementAddr(int index);
		void CorrectElementPointers(LPVOID elementAddr);
	};
}