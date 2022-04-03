#pragma once
#include "pch.h"
#include "IClientComponent.h"
#include "StorageService.h"
#include "StorageFileService.h"
#include "AliveEventReceiverTask.h"

#define STORAGE_MUTEX_NULL_ERROR "Storage mutex was null."
#define ELEMENT_ARGUMENT_NULL_ERROR "Element argument was null."

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

	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const char filePath[FILEPATH_SIZE]);
	virtual HRESULT STDMETHODCALLTYPE CloseStorage();
	virtual HRESULT STDMETHODCALLTYPE Find(Element* element);
	virtual HRESULT STDMETHODCALLTYPE Insert(Element* element);
	virtual HRESULT STDMETHODCALLTYPE Update(Element* element, const void* newPayload, int newPayloadLength);
	virtual HRESULT STDMETHODCALLTYPE Delete(Element* element);
	virtual HRESULT STDMETHODCALLTYPE GetLastError(char* error);
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKey(Element*& element, const void* key, int keyLength);
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKeyPayload(Element*& element, const void* key, int keyLength, const void* payload, int payloadLength);
	virtual HRESULT STDMETHODCALLTYPE PrintElement(Element* element);
	virtual HRESULT STDMETHODCALLTYPE PrintAllElements();
	virtual HRESULT STDMETHODCALLTYPE GetIsStorageClosed();

private: // Fields
	ULONG m_cRef;

	StorageFileService storageFileService;
	StorageService storageService;
	AliveEventReceiverTask aliveEventReceiverTask;

	HANDLE hStorageMutex;
	std::mutex internalMutex;
	char lastError[256];

public: // Getters & Setters
	void setLastError(const char* error);
	char* getLastError();
	void openStorageMutex(const char prefix[FILEPATH_SIZE]);

};

HRESULT ClientComponentCreateInstance(REFIID riid, void** ppv);