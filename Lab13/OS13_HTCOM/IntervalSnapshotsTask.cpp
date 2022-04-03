#include "pch.h"
#include "IntervalSnapshotsTask.h"

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

	std::thread _startIntervalSnap(&IntervalSnapshotsTask::startIntervalSnapshots, this, snapshotService, this->isIntervalSnapOn, secSnapshotInterval);
	_startIntervalSnap.detach();
}

void IntervalSnapshotsTask::startIntervalSnapshots(SnapshotService* snapshotService, std::atomic<bool>* isIntervalSnapOn, int secSnapshotInterval)
{
	try
	{
		if (isIntervalSnapOn->load(std::memory_order_seq_cst))
		{
			do
			{
				std::this_thread::sleep_for(std::chrono::seconds(secSnapshotInterval));
				snapshotService->executeSnap();
			} while (isIntervalSnapOn->load(std::memory_order_seq_cst));
		}

		delete isIntervalSnapOn;
		delete snapshotService;
	}
	catch (const std::exception&)
	{
		delete isIntervalSnapOn;
		// TODO: log
	}
}
