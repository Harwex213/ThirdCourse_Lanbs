#pragma once
#include "pch.h"
#include "Interfaces.h"

namespace StartComponentApi
{
	IStartComponent* Init(std::string loggerPath);
	void CheckOnFailed(IStartComponent* pComponent, HRESULT hResult);
	//void LoadStorage(IStartComponent* pStartComponent, const char fileName[FILEPATH_SIZE], const char directoryPath[FILEPATH_SIZE]);
	//void CloseStorage(IStartComponent* pStartComponent);
	void Dispose(IStartComponent* pStartComponent);
}
