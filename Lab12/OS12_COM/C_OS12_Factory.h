#pragma once
#include "pch.h"
#include <objbase.h>

extern long currentServerLocks;

class C_OS12_Factory : public IClassFactory
{
public:
	C_OS12_Factory();
	~C_OS12_Factory();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUO, const IID& id, void** ppv);
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL b);
private:
	ULONG m_cRef;
};
