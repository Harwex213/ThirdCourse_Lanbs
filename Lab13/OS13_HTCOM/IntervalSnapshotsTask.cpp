#include "pch.h"
#include "IntervalSnapshotsTask.h"
#include "Logger.h"

IntervalSnapshotsTask::IntervalSnapshotsTask()
{
	isIntervalSnapOn = new std::atomic<bool>(false);
}

bool IntervalSnapshotsTask::getIsIntervalSnapOn()
{
	return isIntervalSnapOn->load(std::memory_order_seq_cst);
}

void IntervalSnapshotsTask::setIsIntervalSnapOn(bool value)
{
	isIntervalSnapOn->store(value, std::memory_order_seq_cst);
}

void IntervalSnapshotsTask::start(SnapshotService* snapshotService, int secSnapshotInterval)
{
	this->setIsIntervalSnapOn(true);

	std::thread _startIntervalSnap(&IntervalSnapshotsTask::startIntervalSnapshots, this, snapshotService, secSnapshotInterval);
	_startIntervalSnap.detach();
}

void IntervalSnapshotsTask::startIntervalSnapshots(SnapshotService* snapshotService, int secSnapshotInterval)
{
	logger << "startIntervalSnapshots: start" << std::endl;

	try
	{
		do
		{
			std::this_thread::sleep_for(std::chrono::seconds(secSnapshotInterval));
			if (snapshotService != NULL)
			{
				snapshotService->executeSnap();
			}
		} while (this->getIsIntervalSnapOn());
	}
	catch (const std::exception& error)
	{
		logger << "startIntervalSnapshots: snapshot error - " << error.what() << std::endl;
	}

	delete isIntervalSnapOn;
	logger << "startIntervalSnapshots: finish" << std::endl;
}
