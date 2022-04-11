#include <Windows.h>
#include <iostream>
#include <fstream>

static WCHAR ServiceName[] = L"HarwexStorage";

int main()
{
	SC_HANDLE schService = NULL;
	SC_HANDLE schSCManager = NULL;

	try
	{
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (schSCManager == NULL)
		{
			throw std::exception("OpenSCManager failed");
		}

		schService = OpenService(
			schSCManager,
			ServiceName,
			SERVICE_ALL_ACCESS
		);
		if (schService == NULL)
		{
			throw std::exception("OpenService failed");
		}

		if (DeleteService(schService) == FALSE)
		{
			throw std::exception("DeleteService failed");
		}

		printf_s("Success.\n");
	}
	catch (const std::exception& error)
	{
		printf_s("Service install error: %s. GetLastError() - %d\n", error.what(), GetLastError());
	}

	if (schService != NULL)
	{
		CloseServiceHandle(schService);
	}
	if (schSCManager != NULL)
	{
		CloseServiceHandle(schSCManager);
	}

	system("pause");
}
