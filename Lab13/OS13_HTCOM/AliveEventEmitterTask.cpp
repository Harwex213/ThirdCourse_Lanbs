#include "pch.h"
#include "AliveEventEmitterTask.h"
#include "Logger.h"

AliveEventEmitterTask::AliveEventEmitterTask()
{
	hEvent = NULL;
	isTaskOn = new std::atomic<bool>(false);
}

bool AliveEventEmitterTask::getIsTaskOn()
{
	return isTaskOn->load(std::memory_order_seq_cst);
}

void AliveEventEmitterTask::setIsTaskOn(bool value)
{
	isTaskOn->store(value, std::memory_order_seq_cst);
}

void AliveEventEmitterTask::start(const char filePath[FILEPATH_SIZE])
{
	std::string eventName = filePath; eventName += "-event";
	this->hEvent = CreateEventA(NULL, TRUE, TRUE, eventName.c_str());
	if (this->hEvent == NULL)
	{
		throw std::exception(CREATE_ALIVE_EVENT_ERROR);
	}

	this->setIsTaskOn(true);
	std::thread eventReceiver(&AliveEventEmitterTask::startEmitting, this, this->isTaskOn);
	eventReceiver.detach();
}


void AliveEventEmitterTask::startEmitting(std::atomic<bool>* isTaskOn)
{
#ifdef DEBUG
	printf_s("AliveEventEmitterTask started\n");
#endif // DEBUG

	logger << "AliveEventEmitterTask: start" << std::endl;
	while (isTaskOn->load(std::memory_order_seq_cst))
	{
		ResetEvent(this->hEvent);
		Sleep(50);
		SetEvent(this->hEvent);

#ifdef DEBUG
		printf_s("ALIVE EVENT\n");
#endif // DEBUG
	}
	ResetEvent(this->hEvent);
	logger << "AliveEventEmitterTask: finish" << std::endl;

#ifdef DEBUG
	printf_s("AliveEventEmitterTask finished\n");
#endif // DEBUG
}