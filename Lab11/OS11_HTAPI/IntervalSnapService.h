#pragma once
#include "pch.h"
#include "SnapService.h"

namespace Core
{
	struct IntervalSnapService
	{
	public: // Constructors
		IntervalSnapService();

	private: // Fields
		std::atomic<bool>* isIntervalSnapOn;

	public: // Getters & Setters
		bool getIsIntervalSnapOn();
		void setIsIntervalSnapOn(bool value);

	private: // Private methods

	public: // Public methods
		void startIntervalSnap(SnapService* snapService, int secSnapshotInterval);
	};

	void startIntervalSnapThread(SnapService* snapService, std::atomic<bool>* isIntervalSnapOn, int secSnapshotInterval);
}