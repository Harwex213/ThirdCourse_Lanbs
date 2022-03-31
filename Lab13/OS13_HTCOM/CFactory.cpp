#include "pch.h"
#include "CFactory.h"
#include "Logger.h"

long currentServerLocks = 0;
long currentComponentsAmount = 0;

CFactory::CFactory(CreateInstanceFunc createInstance) : m_cRef(1)
{
	this->createInstance = createInstance;

	logger << "CFactory: Constructor call" << std::endl;
}

CFactory::~CFactory()
{
	logger << "CFactory: Destructor call" << std::endl;
}

HRESULT __stdcall CFactory::QueryInterface(REFIID iid, void** ppv)
{
	logger << "CFactory: QueryInterface call" << std::endl;

	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory)) {
		*ppv = static_cast<IClassFactory*>(this);
	} else {
		*ppv = NULL;
		logger << "CFactory: QueryInterface - E_NOINTERFACE" << std::endl;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	logger << "CFactory: QueryInterface - S_OK" << std::endl;
	return S_OK;
}

ULONG __stdcall CFactory::AddRef()
{
	InterlockedIncrement(&m_cRef);
	logger << "CFactory: AddRef - " << m_cRef << std::endl;
	return m_cRef;
}

ULONG __stdcall CFactory::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		logger << "CFactory: Full release " << std::endl;
		delete this;
		return 0;
	}

	logger << "CFactory: Release - " << m_cRef << std::endl;
	return m_cRef;
}

HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv)
{
	logger << "CFactory: CreateInstance call" << std::endl;

	if (pUnknownOuter != NULL) {
		logger << "CFactory: CreateInstance - CLASS_E_NOAGGREGATION" << std::endl;
		return CLASS_E_NOAGGREGATION;
	}

	HRESULT hResult = createInstance(iid, ppv);
	logger << "CFactory: CreateInstance - finished. HResult: " << hResult << std::endl;
	return hResult;
}

HRESULT __stdcall CFactory::LockServer(BOOL b)
{
	if (b) {
		InterlockedIncrement(&currentServerLocks);
	} else {
		InterlockedDecrement(&currentServerLocks);
	}

	logger << "CFactory: LockServer - " << b << std::endl;
	return S_OK;
}
