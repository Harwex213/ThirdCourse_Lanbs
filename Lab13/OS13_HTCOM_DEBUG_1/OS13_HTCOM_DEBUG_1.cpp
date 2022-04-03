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

    char input[128];
    do
    {
        printf_s("Commands:\n q - to quit START\n");
        std::cin >> input;
        printf_s("\n");

    } while (strcmp(input, "q") != 0);

    hResult = pStartComponent->CloseStorage();
    if (FAILED(hResult))
    {
        char error[256];
        pStartComponent->GetLastError(error);
        throw std::exception(error);
    }

    reinterpret_cast<IUnknown*>(pStartComponent)->Release();
    CoFreeUnusedLibraries();
}

void testClientComponent()
{
    std::string storagePath = proccessPath + "/storage.ht";
    std::string loggerPath = "D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab13\\input\\clients-log\\clientComponent-"
        + std::to_string(GetCurrentProcessId()) + ".log";
    IClientComponent* pClientComponent = NULL;

    setLogger(loggerPath);
    CoInitialize(NULL);
    HRESULT hResult = CoCreateInstance(CLSID_ClientComponent, NULL, CLSCTX_INPROC_SERVER, IID_IClientComponent, (void**)&pClientComponent);
    if (FAILED(hResult))
    {
        throw std::exception("Failed create instance");
    }

    hResult = pClientComponent->OpenStorage(storagePath.c_str());
    if (FAILED(hResult))
    {
        char error[256];
        pClientComponent->GetLastError(error);
        throw std::exception(error);
    }

    Element* element = NULL;
    char input[128];
    do
    {
        printf_s("Commands:\n q - to quit START\n p - print all\n i - test insert\n u - test update\n d - test delete\n f - test find\n");
        std::cin >> input;
        printf_s("\n");

        if (strcmp(input, "p") == 0)
        {
            pClientComponent->PrintAllElements();
            continue;
        }

        pClientComponent->CreateElementWithKeyPayload(element, "key", 4, "payload", 8);

        if (strcmp(input, "i") == 0)
        {
            hResult = pClientComponent->Insert(element);
            if (FAILED(hResult))
            {
                char error[256];
                pClientComponent->GetLastError(error);
                throw std::exception(error);
            }
            printf_s("Element inserted\n");
            continue;
        }

        if (strcmp(input, "u") == 0)
        {
            hResult = pClientComponent->Update(element, "abra kadabra", 13);
            if (FAILED(hResult))
            {
                char error[256];
                pClientComponent->GetLastError(error);
                throw std::exception(error);
            }
            printf_s("Element updated\n");
            continue;
        }

        if (strcmp(input, "d") == 0)
        {
            hResult = pClientComponent->Delete(element);
            if (FAILED(hResult))
            {
                char error[256];
                pClientComponent->GetLastError(error);
                throw std::exception(error);
            }
            printf_s("Element deleted\n");
            continue;
        }

        if (strcmp(input, "f") == 0)
        {
            pClientComponent->Find(element);
            pClientComponent->PrintElement(element);
            continue;
        }

    } while (strcmp(input, "q") != 0);

    hResult = pClientComponent->CloseStorage();
    if (FAILED(hResult))
    {
        char error[256];
        pClientComponent->GetLastError(error);
        throw std::exception(error);
    }

    reinterpret_cast<IUnknown*>(pClientComponent)->Release();
    CoFreeUnusedLibraries();
}

int main()
{
    try
    {
        //testCreateComponent();
        testStartComponent();
        //testClientComponent();

        printf_s("Success.\n");
    }
    catch (const std::exception& error)
    {
        printf_s("Error: %s.\n", error.what());
    }

    system("pause");
}
