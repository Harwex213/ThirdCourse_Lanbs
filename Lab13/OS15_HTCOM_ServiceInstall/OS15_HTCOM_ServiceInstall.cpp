#include <Windows.h>
#include <iostream>
#include <fstream>

static WCHAR ServiceName[] = L"HarwexStorage";
static WCHAR ServicePath[] = L"D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab13\\x64\\Debug\\OS15_HTCOM_Service.exe";

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	SC_HANDLE schService = NULL;
	SC_HANDLE schSCManager = NULL;
	try
	{
		if (argc < 4) {
			throw std::exception("Wrong process arguments. Should be storagePath(char*), snapshotsDirectoryPath(char*), loggerPath(char*) [, user(char*), password(char*) ]");
		}
		WCHAR* storagePath = argv[1];
		WCHAR* snapshotsDirectoryPath = argv[2];
		WCHAR* loggerPath = argv[3];

		std::wstring servicePathFull = ServicePath; servicePathFull += L" ";
		servicePathFull += storagePath; servicePathFull += L" ";
		servicePathFull += snapshotsDirectoryPath; servicePathFull += L" ";
		servicePathFull += loggerPath; servicePathFull += L" ";
		schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
		if (schSCManager == NULL)
		{
			throw std::exception("OpenSCManager failed");
		}

		schService = CreateService(
			schSCManager,
			ServiceName,
			ServiceName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			servicePathFull.c_str(),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL
		);
		if (schService == NULL)
		{
			throw std::exception("CreateService failed");
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
