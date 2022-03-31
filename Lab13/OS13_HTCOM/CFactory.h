#pragma once
#include "pch.h"

extern long currentServerLocks;
extern long currentComponentsAmount;

typedef HRESULT(*CreateInstanceFunc)(REFIID, void**);

class CFactory : public IClassFactory
{
public:
	CFactory(CreateInstanceFunc createInstance);
	~CFactory();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUO, const IID& id, void** ppv);
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL b);
private:
	ULONG m_cRef;
	CreateInstanceFunc createInstance;
};