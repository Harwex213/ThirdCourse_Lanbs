#include <iostream>
#include <Windows.h>
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "ClientComponentApi.h"

#define MAX_AMOUNT 51;

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

		char error[512];
		srand(time(0));
		std::string keyName = "key ";
		int number = 0;
		BOOL result = false;
		while (true)
		{
			number = rand() % MAX_AMOUNT;
			keyName += std::to_string(number);

			printf_s("-----------------------\n");
			printf_s("Attempt to delete element with key %s\n", keyName.c_str());
			result = ClientComponentApi::Delete(pComponent, keyName.c_str(), keyName.size() + 1);
			if (result)
			{
				printf_s("Successfully deleted\n");
			}
			else {
				pComponent->GetLastError(error);
				if (pComponent->GetIsStorageClosed() == S_OK)
				{
					throw std::exception("Program stopped due to closed storage");
				}
			}
			printf_s("-----------------------\n");

			keyName.resize(4);

			Sleep(1000);
		}
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
