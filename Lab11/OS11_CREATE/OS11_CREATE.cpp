#include <iostream>
#include <Windows.h>
#include "htHandle.h"

typedef Core::HTHANDLE* (*Create)(int, int, int, int, const char*);
typedef char* (*GetHTLastError)(Core::HTHANDLE*);

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 6) {
			throw std::exception("Wrong process arguments. Should be fileName(char*), secSnapshotInteval, capacity, maxKeyLength, maxPayloadLength");
		}
		const char* fileName = argv[1];
		int params[4];
		for (int i = 0; i < 4; ++i) {
			params[i] = strtol(argv[i + 2], NULL, 10);
		}

		HMODULE hModule = LoadLibraryA("OS11_HTAPI.dll");
		if (hModule == NULL) {
			throw std::exception("Error load library");
		}
		
		Create createStorage = (Create)GetProcAddress(hModule, "Create");
		if (createStorage == NULL) {
			throw std::exception("Error get proc address of Create()");
		}
		GetHTLastError getHtLastError = (GetHTLastError)GetProcAddress(hModule, "GetHTLastError");
		if (getHtLastError == NULL) {
			throw std::exception("Error get proc address of GetHTLastError()");
		}

		Core::HTHANDLE* htHandle = createStorage(params[1], params[0], params[2], params[3], fileName);
		if (htHandle->isInitError)
		{
			throw std::exception(getHtLastError(htHandle));
		}

		printf_s("HT-Storage Created: fileName = %s, secSnapshotInterval = %d, capacity = %d, maxKeyLength = %d, maxPayloadLength = %d\n",
			fileName, params[0], params[1], params[2], params[3]);
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}
}
