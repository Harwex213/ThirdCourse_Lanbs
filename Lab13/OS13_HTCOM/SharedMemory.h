#pragma once

struct SharedMemory
{
public: // Constructors
	SharedMemory();

private: // Fields
	int currentSize;
	std::atomic<bool> isChangedFromLastSnap;

public: // Getters; Setters;
	int getCurrentSize();
	void incCurrentSize();
	void decCurrentSize();

	bool getIsChangedFromLastSnap();
	void setIsChangedFromLastSnap(bool value);

private: // Private methods

public: // Public methods

};
