#include <iostream>
#include <string>
#include "Interfaces.h"

std::string proccessPath = "../input";

void setLogger(std::string loggerPath)
{
    HANDLE hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, std::to_string(GetCurrentProcessId()).c_str());
    if (hFileMapping == NULL)
    {
        return;
    }
    LPVOID addr = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (addr == NULL)
    {
        return;
    }
    strcpy_s((char*)addr, loggerPath.size() + 1, loggerPath.c_str());
}

void testCreateComponent()
{
    std::string storagePath = proccessPath + "/storage.ht";
    std::string loggerPath = proccessPath + "/createComponent.log";
    ICreateComponent* pCreateComponent = NULL;

    setLogger(loggerPath);
    CoInitialize(NULL);
    HRESULT hResult = CoCreateInstance(CLSID_CreateComponent, NULL, CLSCTX_INPROC_SERVER, IID_ICreateComponent, (void**)&pCreateComponent);
    if (FAILED(hResult))
    {
        throw std::exception("Failed create instance");
    }

    hResult = pCreateComponent->CreateStorage(15, 5, 20, 50, storagePath.c_str());
    if (FAILED(hResult))
    {
        char error[256];
        pCreateComponent->GetLastError(error);
        throw std::exception(error);
    }

    reinterpret_cast<IUnknown*>(pCreateComponent)->Release();

    CoFreeUnusedLibraries();
}

void testStartComponent()
{
    std::string storagePath = proccessPath + "/storage.ht";
    std::string snapshotsDirectoryPath = proccessPath + "/snapshots";
    std::string loggerPath = proccessPath + "/startComponent.log";
    IStartComponent* pStartComponent = NULL;

    setLogger(loggerPath);
    CoInitialize(NULL);
    HRESULT hResult = CoCreateInstance(CLSID_StartComponent, NULL, CLSCTX_INPROC_SERVER, IID_IStartComponent, (void**)&pStartComponent);
    if (FAILED(hResult))
    {
        throw std::exception("Failed create instance");
    }

    hResult = pStartComponent->LoadStorage(storagePath.c_str(), snapshotsDirectoryPath.c_str());
    if (FAILED(hResult))
    {
        char error[256];
        pStartComponent->GetLastError(error);
        throw std::exception(error);
    }

    for (int i = 0; i < 15; i++)
    {
        Sleep(1000);
    }

    hResult = pStartComponent->CloseStorage();
    if (FAILED(hResult))
    {
        char error[256];
        pStartComponent->GetLastError(error);
        throw std::exception(error);
    }

    Sleep(1000);

    reinterpret_cast<IUnknown*>(pStartComponent)->Release();

    CoFreeUnusedLibraries();
}

int main()
{
    try
    {
        //testCreateComponent();
        testStartComponent();

        printf_s("Success.\n");
    }
    catch (const std::exception& error)
    {
        printf_s("Error: %s.\n", error.what());
    }

    //system("pause");
}
