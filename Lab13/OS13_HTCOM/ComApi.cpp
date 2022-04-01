#include "pch.h"
#include "CFactory.h"
#include "Logger.h"
#include "ClientComponent.h"
#include "CreateComponent.h"
#include "StartComponent.h"

struct GUIDComparer
{
	bool operator()(const GUID& Left, const GUID& Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};

std::map<GUID, CreateInstanceFunc, GUIDComparer> clsIdTable = {
	{ CLSID_CreateComponent, reinterpret_cast<CreateInstanceFunc>(CreateComponentCreateInstance) },
	{ CLSID_ClientComponent, reinterpret_cast<CreateInstanceFunc>(ClientComponentCreateInstance) },
	{ CLSID_StartComponent, reinterpret_cast<CreateInstanceFunc>(StartComponentCreateInstance) }
};

STDAPI DllCanUnloadNow()
{
	bool isPossibleUnload = (currentServerLocks == 0) && (currentComponentsAmount == 0);

	logger << "DllCanUnloadNow: " << "Call" << std::endl;
	logger << "DllCanUnloadNow: currentServerLocks - " << currentServerLocks << std::endl;
	logger << "DllCanUnloadNow: currentComponentsAmount - " << currentComponentsAmount << std::endl;
	logger << "DllCanUnloadNow: isPossibleUnload - " << (isPossibleUnload ? "true" : "false") << std::endl;

	return isPossibleUnload ? S_OK : S_FALSE;
}

STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv)
{
	logger << "DllGetClassObject: " << "Call" << std::endl;

	std::map<GUID, CreateInstanceFunc>::iterator it = clsIdTable.find(clsid);
	if (it == clsIdTable.end())
	{
		logger << "DllGetClassObject: " << "CLASS_E_CLASSNOTAVAILABLE" << std::endl;
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	CFactory* pFactory;
	if ((pFactory = new CFactory(it->second)) == NULL) {
		logger << "DllGetClassObject: " << "E_OUTOFMEMORY" << std::endl;
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pFactory->QueryInterface(iid, ppv);
	pFactory->Release();
	logger << "DllGetClassObject: CFactory instantiated. HResult - " << hResult << std::endl;
	return hResult;
}
