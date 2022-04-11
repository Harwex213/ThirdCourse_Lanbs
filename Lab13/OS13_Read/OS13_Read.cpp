#include <iostream>
#include <Windows.h>
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "ClientComponentApi.h"

int main(int argc, char* argv[])
{
	IClientComponent* pComponent = NULL;
	try
	{
		if (argc != 3) {
			throw std::exception("Wrong process arguments. Should be storagePath(char*), loggerPath(char*)");
		}
		const char* storagePath = argv[1];
		std::string loggerPath = argv[2];

		pComponent = ClientComponentApi::Init(loggerPath);
		HRESULT hResult = pComponent->OpenStorage(storagePath);
		ClientComponentApi::CheckOnFailed(pComponent, hResult);
		
		char input[512];
		do
		{
			printf_s("--------------------------------\n");
			hResult = pComponent->PrintAllElements();
			ClientComponentApi::CheckOnFailed(pComponent, hResult);
			printf_s("--------------------------------\n");

			printf_s("Input q to quit\n");
			std::cin >> input;
		} while (strcmp(input, "q") != 0);
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}

	if (pComponent != NULL)
	{
		ClientComponentApi::Dispose(pComponent);
	}
	system("pause");
}
