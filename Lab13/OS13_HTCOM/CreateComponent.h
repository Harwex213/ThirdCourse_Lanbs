#pragma once
#include "pch.h"
#include "ICreateComponent.h"
#include "StorageFileService.h"
#include "StorageService.h"

// {00DB7A85-8321-4530-8D2A-FD4AA3492585}
static const GUID CLSID_CreateComponent =
{ 0xdb7a85, 0x8321, 0x4530, { 0x8d, 0x2a, 0xfd, 0x4a, 0xa3, 0x49, 0x25, 0x85 } };

static const wchar_t* FNAME_CreateComponent = L"CreateComponent.COM";
static const wchar_t* VINDX_CreateComponent = L"CreateComponent.1";
static const wchar_t* PGID_CreateComponent = L"CreateComponent";

extern long currentComponentsAmount;

class CreateComponent : public ICreateComponent
{
public:
	CreateComponent();
	~CreateComponent();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE]);

private:
	ULONG m_cRef;

	StorageFileService storageFileService;
	StorageService storageService;
};

HRESULT CreateComponentCreateInstance(REFIID riid, void** ppv);