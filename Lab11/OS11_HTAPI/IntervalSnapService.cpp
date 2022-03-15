#include "pch.h"
#include "IntervalSnapService.h"

namespace Core
{
	IntervalSnapService::IntervalSnapService()
	{
		isIntervalSnapOn = new std::atomic<bool>(false);
	}

	bool IntervalSnapService::getIsIntervalSnapOn()
	{
		return isIntervalSnapOn->load(std::memory_order_seq_cst);
	}

	void IntervalSnapService::setIsIntervalSnapOn(bool value)
	{
		isIntervalSnapOn->store(value, std::memory_order_seq_cst);
	}

	void Core::IntervalSnapService::startIntervalSnap(SnapService* snapService, int secSnapshotInterval)
	{
		this->setIsIntervalSnapOn(true);

		std::thread _startIntervalSnap(startIntervalSnapThread, snapService, this->isIntervalSnapOn, secSnapshotInterval);
		_startIntervalSnap.detach();
	}

	void startIntervalSnapThread(SnapService* snapService, std::atomic<bool>* isIntervalSnapOn, int secSnapshotInterval)
	{
		if (isIntervalSnapOn->load(std::memory_order_seq_cst))
		{
			do
			{
				std::this_thread::sleep_for(std::chrono::seconds(secSnapshotInterval));
				snapService->executeSnap();
			} while (isIntervalSnapOn->load(std::memory_order_seq_cst));
		}

		delete isIntervalSnapOn;
		delete snapService;
	}
}