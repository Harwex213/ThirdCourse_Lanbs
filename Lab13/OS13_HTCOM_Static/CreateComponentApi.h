#pragma once
#include "pch.h"
#include "Interfaces.h"

namespace CreateComponentApi
{
	ICreateComponent* Init(std::string loggerPath);
	void CreateStorage(ICreateComponent* pCreateComponent, int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE]);
	void Dispose(ICreateComponent* pCreateComponent);
}
