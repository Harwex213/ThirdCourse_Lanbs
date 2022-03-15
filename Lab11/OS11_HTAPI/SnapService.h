#pragma once
#include "pch.h"
#include "FileService.h"

#define SNAPSHOT_DIRECTORY_NAME "snapshots"

#define CREATE_SNAPS_DIRECTORY_ERROR "Cannot create directory for snapshots"
#define CREATE_SNAP_FILE_ERROR "Cannot create snapshot file of storage"
#define WRITE_TO_SNAP_FILE_ERROR "Cannot save storage to opened snapshot file"
#define CLOSE_SNAP_FILE_ERROR "Cannot close snapshot file of storage"

namespace Core
{
	struct SnapService
	{
	public: // Constructors
		SnapService(const char* storageFilePathName, LPVOID storageMemoryStart, DWORD storageMemorySize);

	private: // Fields
		FileService fileService;
		std::string storageFileName;
		std::string storageFilePath;
		std::string snapshotsDirectoryPath;

		LPVOID storageMemoryStart;
		DWORD storageMemorySize;

	public: // Getters & Setters


	private: // Private methods
		std::string createDirectoryForSnaps();
		std::string generateSnapFileName();

	public: // Public methods
		void executeSnap();
	};
}