#include "pch.h"
#include "CreateComponent.h"
#include "Logger.h"
#include "StorageConfig.h"

CreateComponent::CreateComponent() : m_cRef(1)
{
	InterlockedIncrement(&currentComponentsAmount);

	*this->lastError = '\0';

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

void CreateComponent::setLastError(const char* error)
{
	strcpy_s(this->lastError, strlen(error) + 1, error);
}

char* CreateComponent::getLastError()
{
	return this->lastError;
}

HRESULT __stdcall CreateComponent::CreateStorage(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char filePath[FILEPATH_SIZE], const char HTUsersGroupName[FILEPATH_SIZE])
{
	logger << "CreateStorage: call" << std::endl;

	try
	{
		authService.checkHtGroupOnExist(HTUsersGroupName);
		logger << "CreateStorage: users group verified" << std::endl;

		WCHAR userName[512];
		DWORD userNameLength = 512;
		GetUserName(userName, &userNameLength);
		authService.checkUserOnAdmin(userName);
		logger << "CreateStorage: user belonging to admin group verified" << std::endl;

		authService.checkUserOnHtGroups(HTUsersGroupName, userName);
		logger << "CreateStorage: user belonging to HT group verified" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "CreateStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}


	std::string mutexName = filePath; mutexName += "-mutex";
	HANDLE hStorageMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	if (hStorageMutex == NULL)
	{
		setLastError(CREATE_MUTEX_ERROR);
		logger << "CreateStorage: " << CREATE_MUTEX_ERROR << std::endl;
		return E_FAIL;
	}
	logger << "CreateStorage: mutex created" << std::endl;

	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "CreateStorage: mutex taked" << std::endl;

	StorageConfig storageConfig(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, HTUsersGroupName);
	LPVOID addr = NULL;

	try
	{
		addr = this->storageFileService.CreateStorage(filePath, storageConfig.getStorageMemorySize());
		logger << "CreateStorage: storage file created" << std::endl;

		this->storageService.InitializeStorage(addr, storageConfig);
		logger << "CreateStorage: storage initialized" << std::endl;

		this->storageFileService.CloseStorage(addr, storageConfig.getStorageMemorySize());

		logger << "CreateStorage: storage file closed" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		this->storageFileService.ForceCloseStorage(addr, storageConfig.getStorageMemorySize());
		ReleaseMutex(hStorageMutex);
		CloseHandle(hStorageMutex);
		logger << "CreateStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	CloseHandle(hStorageMutex);
	logger << "CreateStorage: mutex release" << std::endl;
	return S_OK;
}

HRESULT __stdcall CreateComponent::GetLastError(char* error)
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
