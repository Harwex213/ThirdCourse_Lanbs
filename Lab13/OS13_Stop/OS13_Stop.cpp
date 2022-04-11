#include <iostream>
#include <Windows.h>
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "ClientComponentApi.h"

int main(int argc, char* argv[])
{
	IClientComponent* pComponent = NULL;
	try
	{
		if (argc < 3) {
			throw std::exception("Wrong process arguments. Should be storagePath(char*), loggerPath(char*) [, user(char*), password(char*) ]");
		}
		const char* storagePath = argv[1];
		std::string loggerPath = argv[2];
		HRESULT hResult = NULL;

		pComponent = ClientComponentApi::Init(loggerPath);
		if (argc == 5)
		{
			const char* user = argv[3];
			const char* password = argv[4];

			hResult = pComponent->OpenStorage(storagePath, user, password);
			ClientComponentApi::CheckOnFailed(pComponent, hResult);

			hResult = pComponent->CheckPermissionOnClose(user, password);
			ClientComponentApi::CheckOnFailed(pComponent, hResult);
		}
		else
		{
			hResult = pComponent->OpenStorage(storagePath);
			ClientComponentApi::CheckOnFailed(pComponent, hResult);

			hResult = pComponent->CheckPermissionOnClose();
			ClientComponentApi::CheckOnFailed(pComponent, hResult);
		}

		std::string closeEventName = "Global\\"; closeEventName += storagePath; closeEventName += "-closeEvent";
		HANDLE hCloseEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, closeEventName.c_str());
		if (hCloseEvent == NULL)
		{
			throw std::exception("Cannot open event to notify to close start");
		}

		SetEvent(hCloseEvent);
		CloseHandle(hCloseEvent);
		
		printf_s("Success.\n");
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
