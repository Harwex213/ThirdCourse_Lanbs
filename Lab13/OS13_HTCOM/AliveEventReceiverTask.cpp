#include "pch.h"
#include "AliveEventReceiverTask.h"
#include "Logger.h"

AliveEventReceiverTask::AliveEventReceiverTask()
{
	hEvent = NULL;
	isTaskOn = new std::atomic<bool>(false);
}

bool AliveEventReceiverTask::getIsTaskOn()
{
	return isTaskOn->load(std::memory_order_seq_cst);
}

void AliveEventReceiverTask::setIsTaskOn(bool value)
{
	isTaskOn->store(value, std::memory_order_seq_cst);
}

void AliveEventReceiverTask::start(const char filePath[FILEPATH_SIZE], ICloseStorage& target)
{
	std::string eventName = filePath; eventName += "-event";
	this->hEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, eventName.c_str());
	if (this->hEvent == NULL)
	{
		throw std::exception(OPEN_ALIVE_EVENT_ERROR);
	}

	this->setIsTaskOn(true);

	std::thread _startIntervalSnap(&AliveEventReceiverTask::startReceiving, this, target, this->isTaskOn);
	_startIntervalSnap.detach();
	Sleep(180);
}

void AliveEventReceiverTask::startReceiving(ICloseStorage& target)
{
	logger << "AliveEventReceiverTask: start" << std::endl;

	DWORD result = 0;
	do
	{
		result = WaitForSingleObject(this->hEvent, INFINITE);
		Sleep(100);
	} while (result == WAIT_OBJECT_0 && this->getIsTaskOn());

	logger << "AliveEventReceiverTask: alive event from center has not been received. Try to close storage" << std::endl;

	try
	{
		target.CloseStorage();
	}
	catch (const std::exception& error)
	{
		logger << "AliveEventReceiverTask: storage closing error - " << error.what() << std::endl;
	}

	this->setIsTaskOn(false);
	CloseHandle(this->hEvent);
}
