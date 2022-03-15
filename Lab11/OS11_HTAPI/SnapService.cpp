#include "pch.h"
#include "SnapService.h"
#include "Helper.h"

namespace Core
{
	SnapService::SnapService(const char* storageFilePathName, LPVOID storageMemoryStart, DWORD storageMemorySize)
	{
		this->storageMemoryStart = storageMemoryStart;
		this->storageMemorySize = storageMemorySize;
		this->storageFileName = Helper::parseFileName(storageFilePathName);
		this->storageFilePath = Helper::parseFilePath(storageFilePathName);
		this->snapshotsDirectoryPath = createDirectoryForSnaps();
	}

	std::string SnapService::createDirectoryForSnaps()
	{
		std::string directoryToCreate = this->storageFilePath + "/";
		directoryToCreate += SNAPSHOT_DIRECTORY_NAME;
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

	std::string SnapService::generateSnapFileName()
	{
		std::string snapFilename = this->storageFilePath + "/" + this->snapshotsDirectoryPath;
		snapFilename += "/" + this->storageFileName;
		snapFilename += "-" + Helper::generateRandomString(12);
		snapFilename += "." + Helper::timeToLocalString(time(NULL));
		snapFilename += ".ht";

		return snapFilename;
	}
	
	void SnapService::executeSnap()
	{
		std::string snapFileName = generateSnapFileName();
		HANDLE hSnapFile = fileService.createFile(snapFileName.c_str());
		if (hSnapFile == INVALID_FILE_HANDLE)
		{
			throw std::exception(CREATE_SNAP_FILE_ERROR);
		}

		if (fileService.writeToFile(hSnapFile, storageMemoryStart, storageMemorySize) == false)
		{
			throw std::exception(WRITE_TO_SNAP_FILE_ERROR);
		}

		if (fileService.closeFile(hSnapFile) == false)
		{
			throw std::exception(CLOSE_SNAP_FILE_ERROR);
		}
	}
}