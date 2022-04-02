#include <iostream>
#include <string>
#include "Interfaces.h"

std::string proccessPath = "../input";
std::string storagePath = proccessPath + "/storage.ht";
std::string loggerPath = proccessPath + "/createComponent.log";

void setLogger()
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
    ICreateComponent* pCreateComponent = NULL;

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

int main()
{
    try
    {
        setLogger();
        testCreateComponent();

        printf_s("Success.\n");
    }
    catch (const std::exception& error)
    {
        printf_s("Error: %s.\n", error.what());
    }

    //system("pause");
}
