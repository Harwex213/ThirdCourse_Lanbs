#pragma once
#include "pch.h"
#include "ICloseStorage.h"
#include "Values.h"

#define OPEN_ALIVE_EVENT_ERROR "Cannot open event object for receiving alive events from center."

struct AliveEventReceiverTask
{
public: // Constructors
	AliveEventReceiverTask();

private: // Fields
	HANDLE hEvent;
	std::atomic<bool>* isTaskOn;

public: // Getters & Setters
	bool getIsTaskOn();
	void setIsTaskOn(bool value);

private: // Private methods
	void startReceiving(ICloseStorage& target);

public: // Public methods
	void start(const char filePath[FILEPATH_SIZE], ICloseStorage& target);
};