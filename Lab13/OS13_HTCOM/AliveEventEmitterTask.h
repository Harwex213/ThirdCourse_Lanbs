#pragma once
#include "pch.h"

#define CREATE_ALIVE_EVENT_ERROR "Cannot create event object for emitting alive events."

struct AliveEventEmitterTask
{
public: // Constructors
	AliveEventEmitterTask();

private: // Fields
	HANDLE hEvent;
	std::atomic<bool>* isTaskOn;

public: // Getters & Setters
	bool getIsTaskOn();
	void setIsTaskOn(bool value);

private: // Private methods
	void startEmitting(std::atomic<bool>* isTaskOn);

public: // Public methods
	void start(const char filePath[FILEPATH_SIZE]);
};