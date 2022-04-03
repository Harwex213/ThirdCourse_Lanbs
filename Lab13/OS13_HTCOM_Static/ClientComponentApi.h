#pragma once
#include "pch.h"
#include "Interfaces.h"

namespace ClientComponentApi
{
	IClientComponent* Init(std::string loggerPath);
	void OpenStorage(IClientComponent* pClientComponent, const char fileName[FILEPATH_SIZE]);
	void CloseStorage(IClientComponent* pClientComponent);
	bool Find(IClientComponent* pClientComponent, const void* key, int keyLength);
	bool Insert(IClientComponent* pClientComponent, const void* key, int keyLength, const void* payload, int payloadLength);
	bool Update(IClientComponent* pClientComponent, const void* key, int keyLength, const void* newPayload, int newPayloadLength);
	bool Delete(IClientComponent* pClientComponent, const void* key, int keyLength);
	void PrintStorage(IClientComponent* pClientComponent);
	void Dispose(IClientComponent* pClientComponent);
}
