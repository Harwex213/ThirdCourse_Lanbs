// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "C_OS12.h"
#include "C_OS12_Factory.h"
#include "Registry.h"

static HMODULE g_hModule = NULL;
std::fstream LogCOM;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		LogCOM.open("D:\\Wordplace\\2_Blue\\1University\\third_course\\OperatingSystems\\Labs\\Lab12\\x64\\Debug\\logCOM.txt", std::fstream::out);
		LOG("LOGCOM: ", "DLL_PROCESS_ATTACH");
		break;
	case DLL_THREAD_ATTACH:
		LOG("LOGCOM: ", "DLL_THREAD_ATTACH");
		break;
	case DLL_THREAD_DETACH:
		LOG("LOGCOM: ", "DLL_THREAD_DETACH");
		break;
	case DLL_PROCESS_DETACH:
		LogCOM.close();
		LOG("LOGCOM: ", "DLL_PROCESS_DETACH");
		break;
    }
    return TRUE;
}

STDAPI DllCanUnloadNow() 
{
	LOG("DllCanUnloadNow - currentServerLocks: ", currentServerLocks);
	LOG("DllCanUnloadNow - currentComponentsAmount: ", currentComponentsAmount);
	bool isPossible = (currentServerLocks == 0) && (currentComponentsAmount == 0);

	LOG("DllCanUnloadNow - isPossible: ", isPossible ? "true" : "false");
	return isPossible ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv) 
{
	C_OS12_Factory* pFactory;
	if (clsid != IID_C_OS12) {
		LOG("DllGetClassObject: ", "CLASS_E_CLASSNOTAVAILABLE");
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	else if ((pFactory = new C_OS12_Factory()) == NULL) {
		LOG("DllGetClassObject: ", "E_OUTOFMEMORY");
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();
	LOG("DllGetClassObject - hr: ", hResult);
	return hResult;
}

STDAPI DllInstall(BOOL b, PCWSTR s) 
{
	LOG("LOGCOM: ", "DllInstall");
	return S_OK;
}

STDAPI DllRegisterServer()
{
	LOG("LOGCOM: ", "DllRegisterServer");
	return RegisterServer(
		g_hModule,
		IID_C_OS12,
		FNAME,
		VINDX,
		PGID
	);
}

STDAPI DllUnregisterServer()
{
	LOG("LOGCOM: ", "DllUnregisterServer");
	return UnregisterServer(
		IID_C_OS12,
		VINDX,
		PGID
	);
}


