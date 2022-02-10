#pragma once
#include <Windows.h>

#define CHAR_MAX_LENGTH 512

namespace HT
{
	struct HTHANDLE
	{
		HTHANDLE();
		HTHANDLE(int capacity, int cecSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH]);

		const char* defaultFileName = "./input/txt";

		int capacity;					// ������� ��������� � ���������� ��������� 
		int secSnapshotInterval;		// ������������� ���������� � ���. 
		int maxKeyLength;				// ������������ ����� �����
		int maxPayloadLength;			// ������������ ����� ������
		char fileName[CHAR_MAX_LENGTH];			    // ��� ����� 

		HANDLE hFile;					// File HANDLE != 0, ���� ���� ������
		HANDLE hFileMapping;			// Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID addr;					// Addr != NULL, ���� mapview ��������  

		time_t snapLastTime;			// ���� ���������� snap'a (time())  
		char lastErrorMessage[CHAR_MAX_LENGTH];		// ��������� �� ��������� ������ ��� 0x00

		void initDefault();
	};
}