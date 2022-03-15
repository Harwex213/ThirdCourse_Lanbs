#include "pch.h"
#include "Api.h"

namespace HT
{
	DLL_API HTHANDLE* Create(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandle = new HTHANDLE(fileName);
		try
		{
			htHandle->CreateStorage(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength);
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			htHandle->setIsInitError(true);
			return htHandle;
		}

		htHandle->setIsInitError(false);
		return htHandle;
	}

	DLL_API HTHANDLE* Load(const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandle = new HTHANDLE(fileName);
		try
		{
			htHandle->LoadStorage();
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			htHandle->setIsInitError(true);
			return htHandle;
		}

		htHandle->setIsInitError(false);
		return htHandle;
	}

	DLL_API HTHANDLE* Open(const char fileName[CHAR_MAX_LENGTH])
	{
		HTHANDLE* htHandle = new HTHANDLE(fileName);
		try
		{
			htHandle->OpenStorage();
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			htHandle->setIsInitError(true);
			return htHandle;
		}

		htHandle->setIsInitError(false);
		return htHandle;
	}

	DLL_API BOOL Snap(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return false;
		}

		try
		{
			htHandle->ExecuteSnap();
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return false;
		}

		return true;
	}

	DLL_API BOOL Close(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return false;
		}

		try
		{
			htHandle->CloseStorage();
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return false;
		}

		return true;
	}

	DLL_API BOOL Insert(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL)
		{
			return false;
		}

		try
		{
			htHandle->Insert(element);
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return false;
		}

		return true;
	}

	DLL_API BOOL Update(HTHANDLE* htHandle, const Element* oldelement, const void* newpayload, int newpayloadlength)
	{
		if (htHandle == NULL)
		{
			return false;
		}

		try
		{
			htHandle->Update(oldelement, newpayload, newpayloadlength);
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return false;
		}

		return true;
	}

	DLL_API BOOL Delete(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL)
		{
			return false;
		}

		try
		{
			htHandle->Delete(element);
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return false;
		}

		return true;
	}

	DLL_API Element* Get(HTHANDLE* htHandle, const Element* element)
	{
		if (htHandle == NULL)
		{
			return NULL;
		}

		Element* getElement = NULL;
		try
		{
			getElement = htHandle->Get(element);
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
		}

		return getElement;
	}

	DLL_API char* GetHTLastError(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return NULL;
		}

		return htHandle->getLastErrorMessage();
	}

	DLL_API void Print(const Element* element)
	{
		if (element == NULL)
		{
			printf_s("Printing elemenet. NULL\n");
			return;
		}
		printf_s("Printing element. %s: %s\n", element->getKey(), element->getPayload());
	}
	
	DLL_API void PrintAllElements(HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return;
		}

		try
		{
			htHandle->PrintAllElements();
		}
		catch (const std::exception& error)
		{
			std::string errorMessage = error.what();
			errorMessage += ". Windows GetLastError: " + std::to_string(GetLastError());
			htHandle->setLastErrorMessage(errorMessage.c_str());
			return;
		}
	}

	DLL_API Element* CreateElementWithKey(const void* key, int keyLength)
	{
		return new Element(key, keyLength);
	}
	
	DLL_API Element* CreateElementWithKeyPayload(const void* key, int keyLength, const void* payload, int payloadLength)
	{
		return new Element(key, keyLength, payload, payloadLength);
	}
	DLL_API Core::StorageConfig* GetStorageConfig(Core::HTHANDLE* htHandle)
	{
		if (htHandle == NULL)
		{
			return NULL;
		}

		return htHandle->getStorageConfig();
	}
}
