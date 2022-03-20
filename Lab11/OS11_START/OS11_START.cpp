#include <iostream>
#include <Windows.h>
#include "htHandle.h"
#include "StorageConfig.h"

typedef Core::HTHANDLE* (*Load)(const char*);
typedef char* (*GetHTLastError)(Core::HTHANDLE*);
typedef Core::StorageConfig* (*GetStorageConfig)(Core::HTHANDLE*);
typedef BOOL(*Close)(Core::HTHANDLE*);
typedef void (*PrintAllElements)(Core::HTHANDLE*);

int main(int argc, char* argv[])
{
	HMODULE hModule = NULL;
	try
	{
		if (argc != 2) {
			throw std::exception("Wrong process arguments. Should be fileName(char*)");
		}
		const char* fileName = argv[1];

		hModule = LoadLibraryA("OS11_HTAPI.dll");
		if (hModule == NULL) {
			throw std::exception("Error load library");
		}

		Load loadStorage = (Load)GetProcAddress(hModule, "Load");
		if (loadStorage == NULL) {
			throw std::exception("Error get proc address of Load()");
		}
		GetHTLastError getHtLastError = (GetHTLastError)GetProcAddress(hModule, "GetHTLastError");
		if (getHtLastError == NULL) {
			throw std::exception("Error get proc address of GetHTLastError()");
		}
		GetStorageConfig getStorageConfig = (GetStorageConfig)GetProcAddress(hModule, "GetStorageConfig");
		if (getStorageConfig == NULL) {
			throw std::exception("Error get proc address of GetStorageConfig()");
		}
		Close closeStorage = (Close)GetProcAddress(hModule, "Close");
		if (closeStorage == NULL) {
			throw std::exception("Error get proc address of Close()");
		}
		PrintAllElements printAllElements = (PrintAllElements)GetProcAddress(hModule, "PrintAllElements");
		if (printAllElements == NULL) {
			throw std::exception("Error get proc address of PrintAllElements()");
		}

		Core::HTHANDLE* htHandle = loadStorage(fileName);
		if (htHandle->isInitError)
		{
			throw std::exception(getHtLastError(htHandle));
		}
		Core::StorageConfig* storageConfig = getStorageConfig(htHandle);

		printf_s("HT-Storage Created: fileName = %s, secSnapshotInterval = %d, capacity = %d, maxKeyLength = %d, maxPayloadLength = %d\n",
			fileName,
			storageConfig->secSnapshotInterval,
			storageConfig->capacity,
			storageConfig->maxKeyLength,
			storageConfig->maxPayloadLength);

		char input[512];
		do
		{
			printf_s("Commands:\n q - to quit START\n p - to print All elements\n");
			std::cin >> input;
			printf_s("\n");

			if (strcmp(input, "p") == 0)
			{
				printAllElements(htHandle);
			}
		} while (strcmp(input, "q") != 0);

		BOOL result = closeStorage(htHandle);
		if (result == FALSE)
		{
			throw std::exception(getHtLastError(htHandle));
		}
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
		if (hModule != NULL && FreeLibrary(hModule) != FALSE)
		{
			printf_s("Successfully unloaded library\n");
		}
	}
}
