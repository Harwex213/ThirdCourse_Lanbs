#include "pch.h"
#include "StartComponentApi.h"
#include "LoggerApi.h"

namespace StartComponentApi
{
	IStartComponent* Init(std::string loggerPath)
	{
		IStartComponent* pComponent = NULL;

		setLogger(loggerPath);
		CoInitialize(NULL);
		HRESULT hResult = CoCreateInstance(CLSID_StartComponent, NULL, CLSCTX_INPROC_SERVER, IID_IStartComponent, (void**)&pComponent);
		if (FAILED(hResult))
		{
			throw std::exception("Failed create instance");
		}

		return pComponent;
	}

	//void LoadStorage(IStartComponent* pStartComponent, const char fileName[FILEPATH_SIZE], const char directoryPath[FILEPATH_SIZE])
	//{
	//	HRESULT hResult = pStartComponent->LoadStorage(fileName, directoryPath);
	//	if (FAILED(hResult))
	//	{
	//		char error[256];
	//		pStartComponent->GetLastError(error);
	//		throw std::exception(error);
	//	}
	//}
	//
	//void CloseStorage(IStartComponent* pStartComponent)
	//{
	//	HRESULT hResult = pStartComponent->CloseStorage();
	//	if (FAILED(hResult))
	//	{
	//		char error[256];
	//		pStartComponent->GetLastError(error);
	//		throw std::exception(error);
	//	}
	//}

	void CheckOnFailed(IStartComponent* pComponent, HRESULT hResult)
	{
		if (FAILED(hResult))
		{
			char error[256];
			pComponent->GetLastError(error);
			throw std::exception(error);
		}
	}
	
	void Dispose(IStartComponent* pStartComponent)
	{
		reinterpret_cast<IUnknown*>(pStartComponent)->Release();
		CoFreeUnusedLibraries();
	}
}
