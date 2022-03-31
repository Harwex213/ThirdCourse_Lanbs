#pragma once
#include "pch.h"
#include "IStartComponent.h"

// {E26D3691-759D-427E-B3D3-697DEF7AA33C}
static const GUID CLSID_StartComponent =
{ 0xe26d3691, 0x759d, 0x427e, { 0xb3, 0xd3, 0x69, 0x7d, 0xef, 0x7a, 0xa3, 0x3c } };

static const wchar_t* FNAME_StartComponent = L"StartComponent.COM";
static const wchar_t* VINDX_StartComponent = L"StartComponent.1";
static const wchar_t* PGID_StartComponent = L"StartComponent";

extern long currentComponentsAmount;

class StartComponent : public IStartComponent
{
public:
	StartComponent();
	~StartComponent();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE LoadStorage();
	virtual HRESULT STDMETHODCALLTYPE CloseStorage();
	virtual HRESULT STDMETHODCALLTYPE ExecuteSnap();

private:
	ULONG m_cRef;
};

HRESULT StartComponentCreateInstance(REFIID riid, void** ppv);