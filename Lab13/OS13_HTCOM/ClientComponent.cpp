#include "pch.h"
#include "ClientComponent.h"
#include "Logger.h"
#include "StorageConfig.h"
#include "Element.h"

ClientComponent::ClientComponent() : m_cRef(1)
{
	InterlockedIncrement(&currentComponentsAmount);

	*this->lastError = '\0';

	logger << "ClientComponent: Constructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

ClientComponent::~ClientComponent()
{
	InterlockedDecrement(&currentComponentsAmount);

	logger << "ClientComponent: Destructor call. currentComponentsAmount - " << currentComponentsAmount << std::endl;
}

HRESULT __stdcall ClientComponent::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == IID_IUnknown || iid == IID_IClientComponent) {
		*ppv = static_cast<IClientComponent*>(this);
	}
	else {
		ppv = NULL;
		logger << "ClientComponent: QueryInterface - E_NOINTERFACE" << std::endl;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	logger << "ClientComponent: QueryInterface - S_OK" << std::endl;
	return S_OK;

	return E_NOTIMPL;
}

ULONG __stdcall ClientComponent::AddRef()
{
	InterlockedIncrement(&m_cRef);
	logger << "ClientComponent: AddRef - " << m_cRef << std::endl;
	return m_cRef;
}

ULONG __stdcall ClientComponent::Release()
{
	if (InterlockedDecrement(&m_cRef) == 0) {
		logger << "ClientComponent: Full release " << std::endl;
		delete this;
		return 0;
	}

	logger << "ClientComponent: Release - " << m_cRef << std::endl;
	return m_cRef;
}

void ClientComponent::setLastError(const char* error)
{
	strcpy_s(this->lastError, strlen(error) + 1, error);
}

char* ClientComponent::getLastError()
{
	return this->lastError;
}

void ClientComponent::openStorageMutex(const char prefix[FILEPATH_SIZE])
{
	if (hStorageMutex)
	{
		CloseHandle(hStorageMutex);
		hStorageMutex = NULL;
		logger << "ClientComponent: old storage mutex destroyed" << std::endl;
	}

	std::string mutexName = prefix; mutexName += "-mutex";
	hStorageMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
	if (hStorageMutex == NULL)
	{
		throw std::exception(CREATE_MUTEX_ERROR);
	}
	logger << "ClientComponent: storage mutex created" << std::endl;
}

HRESULT __stdcall ClientComponent::OpenStorage(const char filePath[FILEPATH_SIZE], const char user[USER_NAME_SIZE], const char password[USER_NAME_SIZE])
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::OpenStorage: call" << std::endl;

	openStorageMutex(filePath);

	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::OpenStorage: storage mutex taked" << std::endl;

	StorageConfig* storageConfig = NULL;
	LPVOID addr = NULL;

	try
	{
		addr = this->storageFileService.OpenStorage(filePath);
		logger << "ClientComponent::OpenStorage: storage file opened" << std::endl;

		this->storageService.ReceiveStorage(addr);
		storageConfig = this->storageService.getStorageConfig();
		logger << "ClientComponent::OpenStorage: storage memory loaded" << std::endl;

		this->authService.loginAsUser(user, password);
		logger << "CreateStorage: user login success" << std::endl;

		this->authService.checkUserOnHtGroups(this->storageService.getStorageConfig()->usersGroupName, user);
		logger << "CreateStorage: user belonging to HT group verified" << std::endl;

		this->aliveEventReceiverTask.start(filePath, *this);
		logger << "ClientComponent::OpenStorage: alive events receiver task started" << std::endl;
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
		logger << "ClientComponent::OpenStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	logger << "ClientComponent::OpenStorage: storage mutex release" << std::endl;
	return S_OK;
}

HRESULT __stdcall ClientComponent::OpenStorage(const char filePath[FILEPATH_SIZE])
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::OpenStorage: call" << std::endl;

	openStorageMutex(filePath);

	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::OpenStorage: storage mutex taked" << std::endl;

	StorageConfig* storageConfig = NULL;
	LPVOID addr = NULL;

	try
	{
		addr = this->storageFileService.OpenStorage(filePath);
		logger << "ClientComponent::OpenStorage: storage file opened" << std::endl;

		this->storageService.ReceiveStorage(addr);
		storageConfig = this->storageService.getStorageConfig();
		logger << "ClientComponent::OpenStorage: storage memory loaded" << std::endl;

		WCHAR userName[512];
		DWORD userNameLength = 512;
		GetUserName(userName, &userNameLength);

		this->authService.checkUserOnHtGroups(this->storageService.getStorageConfig()->usersGroupName, userName);
		logger << "CreateStorage: user belonging to HT group verified" << std::endl;

		this->aliveEventReceiverTask.start(filePath, *this);
		logger << "ClientComponent::OpenStorage: alive events receiver task started" << std::endl;
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
		logger << "ClientComponent::OpenStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	logger << "ClientComponent::OpenStorage: storage mutex release" << std::endl;
	return S_OK;
}

HRESULT __stdcall ClientComponent::CloseStorage()
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::CloseStorage: call" << std::endl;

	try
	{
		this->aliveEventReceiverTask.setIsTaskOn(false);
		logger << "ClientComponent::CloseStorage: receive alive events off" << std::endl;

		this->storageFileService.CloseStorage(this->storageService.getStorageMemoryStart(), this->storageService.getStorageConfig()->getStorageMemorySize());
		logger << "ClientComponent::CloseStorage: storage file closed" << std::endl;

		this->storageService.ClearStorage();
		logger << "ClientComponent::CloseStorage: storage info cleared" << std::endl;

		CloseHandle(hStorageMutex);
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "StartComponent::CloseStorage: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT __stdcall ClientComponent::Find(Element*& element)
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::Find: call" << std::endl;

	if (element == NULL)
	{
		setLastError(ELEMENT_ARGUMENT_NULL_ERROR);
		return E_FAIL;
	}

	if (hStorageMutex == NULL)
	{
		setLastError(STORAGE_MUTEX_NULL_ERROR);
		return E_FAIL;
	}
	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::Find: storage mutex taked" << std::endl;

	try
	{
		element = storageService.findElement(element->getKey(), element->getKeyLength());
		if (element == ELEMENT_NOT_FOUND_VALUE)
		{
			throw std::exception(ELEMENT_NOUT_FOUND_ERROR);
		}
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::Find: error - " << getLastError() << std::endl;
		ReleaseMutex(hStorageMutex);
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	return S_OK;
}

HRESULT __stdcall ClientComponent::Insert(Element* element)
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::Insert: call" << std::endl;

	if (element == NULL)
	{
		setLastError(ELEMENT_ARGUMENT_NULL_ERROR);
		return E_FAIL;
	}

	if (hStorageMutex == NULL)
	{
		setLastError(STORAGE_MUTEX_NULL_ERROR);
		return E_FAIL;
	}
	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::Insert: storage mutex taked" << std::endl;

	try
	{
		storageService.insertElement(element);
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::Insert: error - " << getLastError() << std::endl;
		ReleaseMutex(hStorageMutex);
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	return S_OK;
}

HRESULT __stdcall ClientComponent::Update(Element* element, const void* newPayload, int newPayloadLength)
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::Update: call" << std::endl;

	if (element == NULL)
	{
		setLastError(ELEMENT_ARGUMENT_NULL_ERROR);
		return E_FAIL;
	}

	if (hStorageMutex == NULL)
	{
		setLastError(STORAGE_MUTEX_NULL_ERROR);
		return E_FAIL;
	}
	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::Update: storage mutex taked" << std::endl;

	try
	{
		storageService.updateElement(new Element(element, newPayload, newPayloadLength));
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::Update: error - " << getLastError() << std::endl;
		ReleaseMutex(hStorageMutex);
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	return S_OK;
}

HRESULT __stdcall ClientComponent::Delete(Element* element)
{
	std::lock_guard<std::mutex> lock(this->internalMutex);
	logger << "ClientComponent::Delete: call" << std::endl;

	if (element == NULL)
	{
		setLastError(ELEMENT_ARGUMENT_NULL_ERROR);
		return E_FAIL;
	}

	if (hStorageMutex == NULL)
	{
		setLastError(STORAGE_MUTEX_NULL_ERROR);
		return E_FAIL;
	}
	WaitForSingleObject(hStorageMutex, INFINITE);
	logger << "ClientComponent::Delete: storage mutex taked" << std::endl;

	try
	{
		storageService.deleteElement(element->getKey(), element->getKeyLength());
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::Delete: error - " << getLastError() << std::endl;
		ReleaseMutex(hStorageMutex);
		return E_FAIL;
	}

	ReleaseMutex(hStorageMutex);
	return S_OK;
}

HRESULT __stdcall ClientComponent::GetLastError(char* error)
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

HRESULT __stdcall ClientComponent::CreateElementWithKey(Element*& element, const void* key, int keyLength)
{
	element = new Element(key, keyLength);
	return S_OK;
}

HRESULT __stdcall ClientComponent::CreateElementWithKeyPayload(Element*& element, const void* key, int keyLength, const void* payload, int payloadLength)
{
	element = new Element(key, keyLength, payload, payloadLength);
	return S_OK;
}

HRESULT __stdcall ClientComponent::PrintElement(Element* element)
{
	if (element == NULL)
	{
		printf_s("Element - NULL\n");
	}
	else
	{
		printf_s("Element - %s: %s\n", element->getKey(), element->getPayload());
	}

	return S_OK;
}

HRESULT __stdcall ClientComponent::PrintAllElements()
{
	if (storageService.getIsMemoryReceived() == false)
	{
		setLastError(STORAGE_CLEAR_ERROR);
		logger << "ClientComponent::PrintAllElements: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	for (size_t i = 0; i < storageService.getStorageConfig()->getCapacity(); i++)
	{
		Element* element = storageService.getElement(i);
		PrintElement(element);
	}
	return S_OK;
}

HRESULT __stdcall ClientComponent::GetIsStorageClosed()
{
	bool isOpened = this->storageService.getIsMemoryReceived() && this->storageFileService.getIsMapped();

	return isOpened ? S_FALSE : S_OK;
}

HRESULT __stdcall ClientComponent::CheckPermissionOnClose()
{
	try
	{
		WCHAR userName[512];
		DWORD userNameLength = 512;
		GetUserName(userName, &userNameLength);
		this->authService.checkUserOnAdmin(userName);
		logger << "ClientComponent::CheckPermissionOnClose: user belonging to admin group verified" << std::endl;

		this->authService.checkUserOnHtGroups(this->storageService.getStorageConfig()->usersGroupName, userName);
		logger << "ClientComponent::CheckPermissionOnClose: user belonging to HT group verified" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::CheckPermissionOnClose: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT __stdcall ClientComponent::CheckPermissionOnClose(const char user[USER_NAME_SIZE], const char password[USER_NAME_SIZE])
{
	try
	{
		this->authService.loginAsUser(user, password);
		logger << "ClientComponent::CheckPermissionOnClose: user login success" << std::endl;

		this->authService.checkUserOnAdmin(user);
		logger << "ClientComponent::CheckPermissionOnClose: user belonging to admin group verified" << std::endl;

		this->authService.checkUserOnHtGroups(this->storageService.getStorageConfig()->usersGroupName, user);
		logger << "ClientComponent::CheckPermissionOnClose: user belonging to HT group verified" << std::endl;
	}
	catch (const std::exception& error)
	{
		setLastError(error.what());
		logger << "ClientComponent::CheckPermissionOnClose: error - " << getLastError() << std::endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT ClientComponentCreateInstance(REFIID iid, void** ppv)
{
	ClientComponent* pComponent = new ClientComponent();
	if (pComponent == NULL) {
		logger << "ClientComponent: CreateInstance - E_OUTOFMEMORY" << std::endl;
		return E_OUTOFMEMORY;
	}

	HRESULT hResult = pComponent->QueryInterface(iid, ppv);
	pComponent->Release();

	logger << "ClientComponent: CreateInstance HResult - " << hResult << std::endl;
	return hResult;
}
