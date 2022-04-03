#include "pch.h"
#include "CreateComponentApi.h"
#include "LoggerApi.h"

namespace CreateComponentApi
{
    ICreateComponent* Init(std::string loggerPath)
    {
        ICreateComponent* pCreateComponent = NULL;

        setLogger(loggerPath);
        CoInitialize(NULL);
        HRESULT hResult = CoCreateInstance(CLSID_CreateComponent, NULL, CLSCTX_INPROC_SERVER, IID_ICreateComponent, (void**)&pCreateComponent);
        if (FAILED(hResult))
        {
            throw std::exception("Failed create instance");
        }
        
        return pCreateComponent;
    }

    void CreateStorage(ICreateComponent* pCreateComponent, int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE])
    {
        HRESULT hResult = pCreateComponent->CreateStorage(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, fileName);
        if (FAILED(hResult))
        {
            char error[256];
            pCreateComponent->GetLastError(error);
            throw std::exception(error);
        }
    }

    void Dispose(ICreateComponent* pCreateComponent)
    {
        reinterpret_cast<IUnknown*>(pCreateComponent)->Release();
        CoFreeUnusedLibraries();
    }
}
