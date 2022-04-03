#pragma once
#include "pch.h"
#include "SnapshotService.h"

struct IntervalSnapshotsTask
{
public: // Constructors
	IntervalSnapshotsTask();

private: // Fields
	std::atomic<bool>* isIntervalSnapOn;

public: // Getters & Setters
	bool getIsIntervalSnapOn();
	void setIsIntervalSnapOn(bool value);

private: // Private methods
	void startIntervalSnapshots(SnapshotService* snapService, int secSnapshotInterval);

public: // Public methods
	void start(SnapshotService* snapService, int secSnapshotInterval);
};
