#include <iostream>
#include <Windows.h>
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "StartComponentApi.h"

int main(int argc, char* argv[])
{
	IStartComponent* pComponent = NULL;
	try
	{
		if (argc != 4) {
			throw std::exception("Wrong process arguments. Should be storagePath(char*), snapshotsDirectoryPath(char*), loggerPath(char*)");
		}
		const char* storagePath = argv[1];
		const char* snapshotsDirectoryPath = argv[2];
		std::string loggerPath = argv[3];

		pComponent = StartComponentApi::Init(loggerPath);
		StartComponentApi::LoadStorage(pComponent, storagePath, snapshotsDirectoryPath);

		StorageConfig* storageConfig = NULL;
		pComponent->GetStorageConfig(storageConfig);
		printf_s("HT-Storage Loaded: filePath = %s, loggerPath = %s, snapshotsDirectoryPath = %s, secSnapshotInterval = %d, capacity = %d, maxKeyLength = %d, maxPayloadLength = %d, usersGroupName = %s\n",
			storagePath, loggerPath.c_str(), snapshotsDirectoryPath,
			storageConfig->secSnapshotInterval, storageConfig->capacity, storageConfig->maxKeyLength, storageConfig->maxPayloadLength,
			storageConfig->usersGroupName);

		std::string closeEventName = storagePath; closeEventName += "-closeEvent";
		HANDLE hCloseEvent = CreateEventA(NULL, TRUE, FALSE, closeEventName.c_str());
		if (hCloseEvent == NULL)
		{
			throw std::exception("Cannpt create event for listening close event");
		}

		WaitForSingleObject(hCloseEvent, INFINITE);

		StartComponentApi::CloseStorage(pComponent);
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}

	if (pComponent != NULL)
	{
		StartComponentApi::Dispose(pComponent);
	}
	system("pause");
}
