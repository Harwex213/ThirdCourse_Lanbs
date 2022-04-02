#include <iostream>
#include "Interfaces.h"

void testCreateComponent()
{
    ICreateComponent* pCreateComponent = NULL;

    CoInitialize(NULL);
    HRESULT hResult = CoCreateInstance(CLSID_CreateComponent, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pCreateComponent);
    if (FAILED(hResult))
    {
        throw std::exception("Failed create instance");
    }

    hResult = pCreateComponent->CreateStorage(15, 5, 20, 50, "../input/harwex/storage.ht");
    if (FAILED(hResult))
    {
        char error[256];
        pCreateComponent->GetLastError(error);
        throw std::exception(error);
    }
}

int main()
{
    try
    {
        testCreateComponent();

        printf_s("Success.\n");
    }
    catch (const std::exception& error)
    {
        printf_s("Error: %s.\n", error.what());
    }

    //system("pause");
}
