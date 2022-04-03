#include "pch.h"
#include "SnapshotService.h"
#include "Helper.h"

SnapshotService::SnapshotService(
	const char* storageFilePathName,
	const char* snapshotDirectoryPath,
	LPVOID storageMemoryStart,
	DWORD storageMemorySize,
	SharedMemory* sharedMemory)
{
	this->sharedMemory = sharedMemory;
	this->storageMemoryStart = storageMemoryStart;
	this->storageMemorySize = storageMemorySize;
	this->storageFileName = Helper::parseFileName(storageFilePathName);
	this->storageFilePath = Helper::parseFilePath(storageFilePathName);
	this->snapshotsDirectoryPath = createDirectoryForSnaps(snapshotDirectoryPath);
}

std::string SnapshotService::createDirectoryForSnaps(const char* snapshotDirectoryPath)
{
	std::string directoryToCreate = this->storageFilePath + "/";
	directoryToCreate += snapshotDirectoryPath;
	if (!CreateDirectoryA(directoryToCreate.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		throw std::exception(CREATE_SNAPS_DIRECTORY_ERROR);
	}

	directoryToCreate += "/" + Helper::generateRandomString(12);
	if (!CreateDirectoryA(directoryToCreate.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		throw std::exception(CREATE_SNAPS_DIRECTORY_ERROR);
	}

	return directoryToCreate;
}

std::string SnapshotService::generateSnapFileName()
{
	std::string snapFilename = this->storageFilePath + "/" + this->snapshotsDirectoryPath;
	snapFilename += "/" + this->storageFileName;
	snapFilename += "-" + Helper::generateRandomString(12);
	snapFilename += "." + Helper::timeToLocalString(time(NULL));
	snapFilename += ".ht";

	return snapFilename;
}

void SnapshotService::executeSnap()
{
	if (storageMemoryStart == NULL)
	{
		throw std::exception(MEMORY_NULL_ERROR);
	}
	if (sharedMemory->getIsChangedFromLastSnap() == false)
	{
		return;
	}

	std::string snapFileName = generateSnapFileName();
	HANDLE hSnapFile = CreateFileA(
		snapFileName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);
	if (hSnapFile == INVALID_FILE_HANDLE)
	{
		throw std::exception(CREATE_SNAP_FILE_ERROR);
	}

	if (WriteFile(hSnapFile, storageMemoryStart, storageMemorySize, NULL, NULL) == FALSE)
	{
		throw std::exception(WRITE_TO_SNAP_FILE_ERROR);
	}

	if (CloseHandle(hSnapFile) == FALSE)
	{
		throw std::exception(CLOSE_SNAP_FILE_ERROR);
	}
}
