#include "Service.h"
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "StartComponentApi.h"
#include "sddl.h"

SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS serviceStatus;
std::wofstream traceFile;
char* storagePath;
char* snapshotsDirectoryPath;
std::string loggerPath;
HANDLE hCloseEvent = NULL;

VOID __stdcall ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	serviceStatus.dwServiceType = SERVICE_WIN32;
	serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;

	if ((hServiceStatus = RegisterServiceCtrlHandler(ServiceName, ServiceHandler)) == FALSE)
	{
		trace("ServiceMain::RegisterServiceCtrlHandler failed. GetLastError() - ", GetLastError());
		return;
	}

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;
	if (SetServiceStatus(hServiceStatus, &serviceStatus) == FALSE)
	{
		trace("ServiceMain::RegisterServiceCtrlHandler failed. GetLastError() - ", GetLastError());
		return;
	}

	IStartComponent* pComponent = NULL;
	try
	{
		HRESULT hResult = NULL;
		pComponent = StartComponentApi::Init(loggerPath);
		trace("ServiceMain: HT-Storage COM object initialized");

		hResult = pComponent->LoadStorage(storagePath, snapshotsDirectoryPath);
		StartComponentApi::CheckOnFailed(pComponent, hResult);

		char buffer[512];
		StorageConfig* storageConfig = NULL;
		pComponent->GetStorageConfig(storageConfig);
		sprintf_s(buffer, 512, "ServiceMain: HT-Storage loaded - filePath = %s, loggerPath = %s, snapshotsDirectoryPath = %s, secSnapshotInterval = %d, capacity = %d, maxKeyLength = %d, maxPayloadLength = %d, usersGroupName = %s",
			storagePath, loggerPath.c_str(), snapshotsDirectoryPath,
			storageConfig->secSnapshotInterval, storageConfig->capacity, storageConfig->maxKeyLength, storageConfig->maxPayloadLength,
			storageConfig->usersGroupName);
		trace(buffer);

		const wchar_t* sdd = L"D:"
			L"(D;OICI;GA;;;BG)" //Deny guests
			L"(D;OICI;GA;;;AN)" //Deny anonymous
			L"(A;OICI;GA;;;AU)" //Allow read, write and execute for Users
			L"(A;OICI;GA;;;BA)"; //Allow all for Administrators
		SECURITY_ATTRIBUTES SA;
		ZeroMemory(&SA, sizeof(SA));
		SA.nLength = sizeof(SA);
		ConvertStringSecurityDescriptorToSecurityDescriptor(
			sdd,
			SDDL_REVISION_1,
			&SA.lpSecurityDescriptor,
			NULL);
		std::string closeEventName = "Global\\"; closeEventName += storagePath; closeEventName += "-closeEvent";
		hCloseEvent = CreateEventA(&SA, TRUE, FALSE, closeEventName.c_str());
		if (hCloseEvent == NULL)
		{
			throw std::exception("Cannot create event for listening close event");
		}
		trace("ServiceMain: start listening close event");

		while (WaitForSingleObject(hCloseEvent, 0) != WAIT_OBJECT_0)
		{
			Sleep(3000);
		}

		ResetEvent(hCloseEvent);
		CloseHandle(hCloseEvent);

		hResult = pComponent->CloseStorage();
		StartComponentApi::CheckOnFailed(pComponent, hResult);
		trace("ServiceMain: HT-Storage closed");
	}
	catch (const std::exception& error)
	{
		trace("ServiceMain: HT-Storage error - ", error.what());
	}
	if (pComponent != NULL)
	{
		StartComponentApi::Dispose(pComponent);
	}

	trace("ServiceMain::Service is stopped.");
	trace("----------------------------------------");
	serviceStatus.dwCurrentState = SERVICE_STOPPED;
	serviceStatus.dwWin32ExitCode = 0;
	serviceStatus.dwCheckPoint = 0;
	serviceStatus.dwWaitHint = 0;
	if (SetServiceStatus(hServiceStatus, &serviceStatus) == FALSE)
	{
		trace("ServiceHandler::SetServiceStatus failed. GetLastError() - ", GetLastError());
	}
}

VOID __stdcall ServiceHandler(DWORD fdwControl)
{
	trace("ServiceHandler: requested control code - ", fdwControl);
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		trace("ServiceHandler: set service status - SERVICE_STOP_PENDING");
		serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
		serviceStatus.dwCheckPoint = 0;
		serviceStatus.dwWaitHint = 0;
		if (SetServiceStatus(hServiceStatus, &serviceStatus) == FALSE)
		{
			trace("ServiceHandler::SetServiceStatus failed. GetLastError() - ", GetLastError());
		}
		SetEvent(hCloseEvent);
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		trace("ServiceHandler: Unrecognized control code - ", fdwControl);
		return;
		break;
	}
}
