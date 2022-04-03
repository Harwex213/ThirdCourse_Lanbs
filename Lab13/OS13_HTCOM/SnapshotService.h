#pragma once
#include "pch.h"
#include "Values.h"
#include "StorageConfig.h"
#include "SharedMemory.h"

#define MEMORY_NULL_ERROR "Cannot create snapshot due to storage memory was null"
#define CREATE_SNAPS_DIRECTORY_ERROR "Cannot create directory for snapshots"
#define CREATE_SNAP_FILE_ERROR "Cannot create snapshot file of storage"
#define WRITE_TO_SNAP_FILE_ERROR "Cannot save storage to opened snapshot file"
#define CLOSE_SNAP_FILE_ERROR "Cannot close snapshot file of storage"

struct SnapshotService
{
public: // Constructors
	SnapshotService(
		const char* storageFilePathName,
		const char* snapshotDirectoryPath, 
		LPVOID storageMemoryStart, 
		DWORD storageMemorySize, 
		SharedMemory* sharedMemory);

private: // Fields
	std::string storageFileName;
	std::string storageFilePath;
	std::string snapshotsDirectoryPath;

	LPVOID storageMemoryStart;
	DWORD storageMemorySize;
	SharedMemory* sharedMemory;

public: // Getters & Setters


private: // Private methods
	std::string createDirectoryForSnaps(const char* snapshotDirectoryPath);
	std::string generateSnapFileName();

public: // Public methods
	void executeSnap();
};
