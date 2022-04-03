#include "pch.h"
#include "StartComponent.h"
#include "Logger.h"
#include "StorageConfig.h"
#include "Element.h"
#include "SharedMemory.h"


StartComponent::StartComponent() : m_cRef(1)
{
	InterlockedIncrement(&currentComponentsAmount);

	snapshotService = NULL;
	hStorageMutex = NULL;
	*this->lastError = '\0';

	logger << "StartComponent: Constructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

StartComponent::~StartComponent()
{
	InterlockedDecrement(&currentComponentsAmount);

	logger << "StartComponent: Destructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

HRESULT __stdcall StartComponent::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == IID_IUnknown || iid == IID_IStartComponent) {
		*ppv = static_cast<IStartComponent*>(this);
	}
	else {
		ppv = NULL;
		logger << "StartComponent: QueryInterface - E_NOINTERFACE" << std::endl;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	logger << "StartComponent: QueryInterface - S_OK" << std::endl;
	return S_OK;

	return E_NOTIMPL;
}

ULONG __stdcall StartComponent::AddRef()
{
	InterlockedIncrement(&m_cRef);
	logger << "StartComponent: AddRef - " << m_cRef << std::endl;
	return m_cRef;
}

ULONG __stdcall StartComponent::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		logger << "StartComponent: Full release " << std::endl;
		delete this;
		return 0;
	}

	logger << "StartComponent: Release - " << m_cRef << std::endl;
	return m_cRef;
}

void StartComponent::setLastError(const char* error)
{
	strcpy_s(this->lastError, strlen(error) + 1, error);
}

char* StartComponent::getLastError()
{
	return this->lastError;
}

void StartComponent::openStorageMutex(const char prefix[FILEPATH_SIZE])
{
	if (hStorageMutex)
	{
		CloseHandle(hStorageMutex);
		hStorageMutex = NULL;
		logger << "StartComponent: old storage mutex destroyed" << std::endl;
	}

	std::string mutexName = prefix; mutexName += "-mutex";
	HANDLE hStorageMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	if (hStorageMutex == NULL)
	{
		throw std::exception(CREATE_MUTEX_ERROR);
	}
	logger << "StartComponent: storage mutex created" << std::endl;
}


HRESULT __stdcall StartComponent::LoadStorage(const char filePath[FILEPATH_SIZE], const char snapshotsDirectoryPath[FILEPATH_SIZE])
{
	logger << "StartComponent::LoadStorage: call" << std::endl;

	openStorageMutex(filePath);

	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "StartComponent::LoadStorage: storage mutex taked" << std::endl;

	StorageConfig* storageConfig = NULL;
	LPVOID addr = NULL;

	try
	{
		addr = this->storageFileService.LoadStorage(filePath, ALLOC_ALL_MEMORY_VALUE);
		logger << "StartComponent::LoadStorage: storage file loaded" << std::endl;

		this->storageService.ReceiveStorage(addr);
		storageConfig = this->storageService.getStorageConfig();
		logger << "StartComponent::LoadStorage: storage info loaded" << std::endl;

		this->snapshotService = new SnapshotService(
			filePath, 
			snapshotsDirectoryPath, 
			this->storageService.getStorageMemoryStart(), 
			storageConfig->getStorageMemorySize(),
			this->storageService.getSharedMemory());
		logger << "StartComponent::LoadStorage: snapshotService initialized" << std::endl;

		this->intervalSnapshotTask.start(this->snapshotService, storageConfig->getSecSnapshotInterval());
		logger << "StartComponent::LoadStorage: interval snapshots task started" << std::endl;

		this->aliveEventEmitterTask.start(filePath);
		logger << "StartComponent::LoadStorage: alive events emitter task started" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		if (storageConfig != NULL)
		{
			this->storageFileService.ForceCloseStorage(addr, storageConfig->getStorageMemorySize());
		}
		ReleaseMutex(hStorageMutex);
		CloseHandle(hStorageMutex);
		logger << "StartComponent::LoadStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	CloseHandle(hStorageMutex);
	logger << "CreateStorage: storage mutex release" << std::endl;
	return S_OK;
}

HRESULT __stdcall StartComponent::CloseStorage()
{
	logger << "StartComponent::CloseStorage: call" << std::endl;

	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "StartComponent::CloseStorage: storage mutex taked" << std::endl;

	try
	{
		this->snapshotService->executeSnap();
		logger << "StartComponent::CloseStorage: last snapshot" << std::endl;

		this->intervalSnapshotTask.setIsIntervalSnapOn(false);
		logger << "StartComponent::CloseStorage: set interval snapshots off" << std::endl;

		delete this->snapshotService;
		this->snapshotService = NULL;

		this->storageFileService.CloseStorage(this->storageService.getStorageMemoryStart(), this->storageService.getStorageConfig()->getStorageMemorySize());
		logger << "StartComponent::CloseStorage: storage file closed" << std::endl;

		this->storageService.ClearStorage();
		logger << "StartComponent::CloseStorage: storage info cleared" << std::endl;

		this->aliveEventEmitterTask.setIsTaskOn(false);
		logger << "StartComponent::CloseStorage: set alive events off" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		ReleaseMutex(hStorageMutex);
		CloseHandle(hStorageMutex);
		logger << "StartComponent::CloseStorage: error - " << getLastError() << std::endl;
	}

	ReleaseMutex(hStorageMutex);
	CloseHandle(hStorageMutex);
	logger << "StartComponent::CloseStorage: storage mutex release" << std::endl;
	return S_OK;
}

HRESULT __stdcall StartComponent::ExecuteSnap()
{
	return E_NOTIMPL;
}

HRESULT __stdcall StartComponent::GetLastError(char* error)
{
	try
	{
		strcpy_s(error, strlen(lastError) + 1, lastError);
		return S_OK;
	}
	catch (const std::exception&)
	{
		return E_FAIL;
	}
}

HRESULT StartComponentCreateInstance(REFIID iid, void** ppv)
{
	StartComponent* pComponent = new StartComponent();
	if (pComponent == NULL) {
		logger << "StartComponent: CreateInstance - E_OUTOFMEMORY" << std::endl;
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pComponent->QueryInterface(iid, ppv);
	pComponent->Release();

	logger << "StartComponent: CreateInstance HResult - " << hResult << std::endl;
	return hResult;
}
