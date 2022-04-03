#include "pch.h"
#include "ClientComponentApi.h"
#include "LoggerApi.h"

namespace ClientComponentApi
{

	IClientComponent* Init(std::string loggerPath)
	{
		IClientComponent* pComponent = NULL;

		setLogger(loggerPath);
		CoInitialize(NULL);
		HRESULT hResult = CoCreateInstance(CLSID_ClientComponent, NULL, CLSCTX_INPROC_SERVER, IID_IClientComponent, (void**)&pComponent);
		if (FAILED(hResult))
		{
			throw std::exception("Failed create instance");
		}

		return pComponent;
	}

	void OpenStorage(IClientComponent* pClientComponent, const char fileName[FILEPATH_SIZE])
	{
		HRESULT hResult = pClientComponent->OpenStorage(fileName);
		if (FAILED(hResult))
		{
			char error[256];
			pClientComponent->GetLastError(error);
			throw std::exception(error);
		}
	}

	void CloseStorage(IClientComponent* pClientComponent)
	{
		HRESULT hResult = pClientComponent->CloseStorage();
		if (FAILED(hResult))
		{
			char error[256];
			pClientComponent->GetLastError(error);
			throw std::exception(error);
		}
	}

	bool Find(IClientComponent* pClientComponent, const void* key, int keyLength)
	{
		Element* element = NULL;
		pClientComponent->CreateElementWithKey(element, key, keyLength);

		HRESULT hResult = pClientComponent->Insert(element);

		return SUCCEEDED(hResult) ? true : false;
	}

	bool Insert(IClientComponent* pClientComponent, const void* key, int keyLength, const void* payload, int payloadLength)
	{
		Element* element = NULL;
		pClientComponent->CreateElementWithKeyPayload(element, key, keyLength, payload, payloadLength);

		HRESULT hResult = pClientComponent->Insert(element);

		return SUCCEEDED(hResult) ? true : false;
	}

	bool Update(IClientComponent* pClientComponent, const void* key, int keyLength, const void* newPayload, int newPayloadLength)
	{
		Element* element = NULL;
		pClientComponent->CreateElementWithKey(element, key, keyLength);

		HRESULT hResult = pClientComponent->Update(element, newPayload, newPayloadLength);

		return SUCCEEDED(hResult) ? true : false;
	}

	bool Delete(IClientComponent* pClientComponent, const void* key, int keyLength)
	{
		Element* element = NULL;
		pClientComponent->CreateElementWithKey(element, key, keyLength);

		HRESULT hResult = pClientComponent->Delete(element);

		return SUCCEEDED(hResult) ? true : false;
	}

	void PrintStorage(IClientComponent* pClientComponent)
	{
		pClientComponent->PrintAllElements();
	}

	void Dispose(IClientComponent* pClientComponent)
	{
		reinterpret_cast<IUnknown*>(pClientComponent)->Release();
		CoFreeUnusedLibraries();
	}
}