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

	std::thread _startIntervalSnap(&IntervalSnapshotsTask::startIntervalSnapshots, this, snapshotService, this->isIntervalSnapOn, secSnapshotInterval);
	_startIntervalSnap.detach();
}

void IntervalSnapshotsTask::startIntervalSnapshots(SnapshotService* snapshotService, std::atomic<bool>* isTaskOn, int secSnapshotInterval)
{
#ifdef DEBUG
	printf_s("IntervalSnapshotsTask started\n");
#endif // DEBUG

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
		} while (isTaskOn->load(std::memory_order_seq_cst));
	}
	catch (const std::exception& error)
	{
		logger << "startIntervalSnapshots: snapshot error - " << error.what() << std::endl;

#ifdef DEBUG
		printf_s("IntervalSnapshotsTask storage closing error\n");
#endif // DEBUG
	}

	logger << "startIntervalSnapshots: finish" << std::endl;
#ifdef DEBUG
	printf_s("IntervalSnapshotsTask finished\n");
#endif // DEBUG
}
