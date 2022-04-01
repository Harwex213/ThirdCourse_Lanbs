#pragma once
#include "pch.h"

struct SnapshotService
{
public: // Constructors
	SnapshotService(const char* storageFilePathName, LPVOID storageMemoryStart, DWORD storageMemorySize);

private: // Fields
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
