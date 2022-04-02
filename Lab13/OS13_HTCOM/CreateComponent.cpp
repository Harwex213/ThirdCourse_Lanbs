#include "pch.h"
#include "CreateComponent.h"
#include "Logger.h"
#include "StorageConfig.h"

CreateComponent::CreateComponent() : m_cRef(1)
{
	InterlockedIncrement(&currentComponentsAmount);

	logger << "CreateComponent: Constructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

CreateComponent::~CreateComponent()
{
	InterlockedDecrement(&currentComponentsAmount);

	logger << "CreateComponent: Destructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

HRESULT __stdcall CreateComponent::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == IID_IUnknown || iid == IID_ICreateComponent) {
		*ppv = static_cast<ICreateComponent*>(this);
	}
	else {
		ppv = NULL;
		logger << "CreateComponent: QueryInterface - E_NOINTERFACE" << std::endl;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	logger << "CreateComponent: QueryInterface - S_OK" << std::endl;
	return S_OK;

	return E_NOTIMPL;
}

ULONG __stdcall CreateComponent::AddRef()
{
	InterlockedIncrement(&m_cRef);
	logger << "CreateComponent: AddRef - " << m_cRef << std::endl;
	return m_cRef;
}

ULONG __stdcall CreateComponent::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		logger << "CreateComponent: Full release " << std::endl;
		delete this;
		return 0;
	}

	logger << "CreateComponent: Release - " << m_cRef << std::endl;
	return m_cRef;
}

HRESULT __stdcall CreateComponent::CreateStorage(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char filePath[FILEPATH_SIZE])
{
	std::string mutexName = filePath; mutexName += "-mutex";
	HANDLE hMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	if (hMutex == NULL)
	{
		logger << "CreateStorage: CreateMutexA error" << std::endl;
		return S_FALSE;
	}

	WaitForSingleObject(hMutex, INFINITE);

	StorageConfig storageConfig(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength);
	LPVOID addr = NULL;

	try
	{
		addr = this->storageFileService.CreateStorage(filePath, storageConfig.getStorageMemorySize());

		this->storageService.InitializeStorage(addr, storageConfig);

		this->storageFileService.CloseStorage(addr, storageConfig.getStorageMemorySize());
	}
	catch (const std::exception&)
	{
		this->storageFileService.ForceCloseStorage(addr, storageConfig.getStorageMemorySize());
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return S_FALSE;
	}

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
	return S_OK;
}

HRESULT CreateComponentCreateInstance(REFIID iid, void** ppv)
{
	CreateComponent* pComponent = new CreateComponent();
	if (pComponent == NULL) {
		logger << "CreateComponent: CreateInstance - E_OUTOFMEMORY" << std::endl;
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pComponent->QueryInterface(iid, ppv);
	pComponent->Release();

	logger << "CreateComponent: CreateInstance HResult - " << hResult << std::endl;
	return hResult;
}
