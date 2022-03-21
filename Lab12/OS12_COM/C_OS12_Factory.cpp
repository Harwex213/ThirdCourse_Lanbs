#include "pch.h"
#include "C_OS12_Factory.h"
#include "C_OS12.h"

long currentServerLocks = 0;

C_OS12_Factory::C_OS12_Factory() : m_cRef(1)
{
	;
	LOG("C_OS12 Factory. ", "Constructor call");
}

C_OS12_Factory::~C_OS12_Factory()
{
	;
	LOG("C_OS12 Factory. ", "Destructor call");
}

HRESULT __stdcall C_OS12_Factory::QueryInterface(REFIID iid, void** ppv)
{
	LOG("C_OS12 Factory. QueryInterface - ", "START");
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory)) {
		*ppv = static_cast<IClassFactory*>(this);
	}
	else {
		*ppv = NULL;
		LOG("C_OS12 Factory. QueryInterface - ", "E_NOINTERFACE");
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	LOG("C_OS12 Factory. QueryInterface - ", "S_OK");
	return S_OK;
}

ULONG __stdcall C_OS12_Factory::AddRef()
{
	;
	InterlockedIncrement(&m_cRef);
	LOG("C_OS12 Factory. AddRef: ", m_cRef);
	return m_cRef;
}

ULONG __stdcall C_OS12_Factory::Release()
{
	;
	if (InterlockedDecrement(&m_cRef) == 0) {
		LOG("C_OS12 Factory. ", "Full release");
		delete this;
		return 0;
	}
	LOG("C_OS12 Factory. Release: ", m_cRef);
	return m_cRef;
}

HRESULT __stdcall C_OS12_Factory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv)
{
	;
	if (pUnknownOuter != NULL) {
		LOG("C_OS12 Factory. CreateInstance: ", "CLASS_E_NOAGGREGATION");
		return CLASS_E_NOAGGREGATION;
	}

	C_OS12* pComponent = new C_OS12();
	if (pComponent == NULL) {
		LOG("C_OS12 Factory. CreateInstance: ", "E_OUTOFMEMORY");
		return E_OUTOFMEMORY;
	}
	
	HRESULT hResult = pComponent->QueryInterface(iid, ppv);

	pComponent->Release();
	LOG("C_OS12 Factory. CreateInstance hr: ", hResult);
	return hResult;
}

HRESULT __stdcall C_OS12_Factory::LockServer(BOOL b)
{
	if (b) {
		InterlockedIncrement(&currentServerLocks);
	}
	else {
		InterlockedDecrement(&currentServerLocks);
	}
	return S_OK;
}
