#include <iostream>
#include "HtHandle.h"
#include "Element.h"

typedef Core::HTHANDLE* (*Open)(const char*);
typedef BOOL(*Delete)(Core::HTHANDLE*, Core::Element*);
typedef char* (*GetHTLastError)(Core::HTHANDLE*);
typedef Core::Element* (*CreateElementWithKey)(const void*, int);

#define MAX_AMOUNT 51;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2) {
			throw std::exception("Wrong process arguments. Should be fileName(char*)");
		}
		const char* fileName = argv[1];

		HMODULE hModule = LoadLibraryA("OS11_HTAPI.dll");
		if (hModule == NULL) {
			throw std::exception("Error load library");
		}

		Open openStorage = (Open)GetProcAddress(hModule, "Open");
		if (openStorage == NULL) {
			throw std::exception("Error get proc address of Open()");
		}
		GetHTLastError getHtLastError = (GetHTLastError)GetProcAddress(hModule, "GetHTLastError");
		if (getHtLastError == NULL) {
			throw std::exception("Error get proc address of GetHTLastError()");
		}
		Delete deleteEl = (Delete)GetProcAddress(hModule, "Delete");
		if (deleteEl == NULL) {
			throw std::exception("Error get proc address of Delete()");
		}
		CreateElementWithKey createEl = (CreateElementWithKey)GetProcAddress(hModule, "CreateElementWithKey");
		if (createEl == NULL) {
			throw std::exception("Error get proc address of CreateElementWithKeyPayload()");
		}

		Core::HTHANDLE* htHandle = openStorage(fileName);
		if (htHandle->isInitError)
		{
			throw std::exception(getHtLastError(htHandle));
		}

		srand(time(0));
		std::string keyName = "key ";
		std::string payloadName = "payload ";
		int number = 0;
		BOOL result = false;
		while (true)
		{
			number = rand() % MAX_AMOUNT;
			keyName += std::to_string(number);
			payloadName += std::to_string(number);

			printf_s("-----------------------\n");
			printf_s("Attempt to update element %s:%s\n", keyName.c_str(), payloadName.c_str());
			result = deleteEl(htHandle, createEl(keyName.c_str(), keyName.size() + 1));
			if (result)
			{
				printf_s("Successfully update element %s:%s\n", keyName.c_str(), payloadName.c_str());
			}
			else {
				printf_s("%s\n", getHtLastError(htHandle));
			}
			printf_s("-----------------------\n");

			keyName.resize(4);
			payloadName.resize(8);

			Sleep(1000);
		}
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}

	system("pause");
}
