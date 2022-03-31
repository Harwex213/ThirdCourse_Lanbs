#pragma once
#include "pch.h"
#include "IClientComponent.h"

// {C9DCCD71-3627-41B2-A434-343C5E661089}
static const GUID CLSID_ClientComponent =
{ 0xc9dccd71, 0x3627, 0x41b2, { 0xa4, 0x34, 0x34, 0x3c, 0x5e, 0x66, 0x10, 0x89 } };

static const wchar_t* FNAME_ClientComponent = L"ClientComponent.COM";
static const wchar_t* VINDX_ClientComponent = L"ClientComponent.1";
static const wchar_t* PGID_ClientComponent = L"ClientComponent";

extern long currentComponentsAmount;

class ClientComponent : public IClientComponent
{
public:
	ClientComponent();
	~ClientComponent();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE OpenStorage();
	virtual HRESULT STDMETHODCALLTYPE CloseStorage();
	virtual HRESULT STDMETHODCALLTYPE Find();
	virtual HRESULT STDMETHODCALLTYPE Insert();
	virtual HRESULT STDMETHODCALLTYPE Update();
	virtual HRESULT STDMETHODCALLTYPE Delete();

private:
	ULONG m_cRef;
};

HRESULT ClientComponentCreateInstance(REFIID riid, void** ppv);