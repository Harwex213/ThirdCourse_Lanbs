#include "pch.h"
#include "CFactory.h"
#include "Logger.h"

struct GUIDComparer
{
	bool operator()(const GUID& Left, const GUID& Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};

// TODO: Fill CLSID of Components
std::map<GUID, CreateInstanceFunc, GUIDComparer> slcIdTable = {
	//{ IID_C_OS12, reinterpret_cast<CreateInstanceFunc>(C_OS12_CreateInstance) },
	//{ IID_C_Test, reinterpret_cast<CreateInstanceFunc>(C_Test_CreateInstance) }
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

	std::map<GUID, CreateInstanceFunc>::iterator it = slcIdTable.find(clsid);
	if (it == slcIdTable.end())
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
