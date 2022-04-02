#include "pch.h"
#include "SelfRegistration.h"
#include "Logger.h"
#include "ClientComponent.h"
#include "CreateComponent.h"
#include "StartComponent.h"

// TODO: Array of components info: clsid, fname, vindx, pgid

void guidToChar(GUID guid, char* input) {
	sprintf_s(input, 77, "Guid = {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

STDAPI DllInstall(BOOL b, PCWSTR s)
{
	logger << "DllInstall: Call" << std::endl;
	return S_OK;
}

STDAPI DllRegisterServer()
{
	char input[77];
	HRESULT hResult = NULL;
	logger << "DllRegisterServer: " << "Start" << std::endl;

	// TODO: Register through iteration over array
	hResult = RegisterServer(
		g_hModule,
		CLSID_CreateComponent,
		FNAME_CreateComponent,
		VINDX_CreateComponent,
		PGID_CreateComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllRegisterServer: CreateComponent registration failed. HResult - " << hResult << std::endl;
		return hResult;
	}
	guidToChar(CLSID_CreateComponent, input);
	logger << "DllRegisterServer: CreateComponent registration - " << input << std::endl;

	hResult = RegisterServer(
		g_hModule,
		CLSID_ClientComponent,
		FNAME_ClientComponent,
		VINDX_ClientComponent,
		PGID_ClientComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllRegisterServer: ClientComponent registration failed. HResult - " << hResult << std::endl;
		return hResult;
	}
	guidToChar(CLSID_ClientComponent, input);
	logger << "DllRegisterServer: CreateComponent registration - " << input << std::endl;

	hResult = RegisterServer(
		g_hModule,
		CLSID_StartComponent,
		FNAME_StartComponent,
		VINDX_StartComponent,
		 PGID_StartComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllRegisterServer: StartComponent registration failed. HResult - " << hResult << std::endl;
		return hResult;
	}
	guidToChar(CLSID_StartComponent, input);
	logger << "DllRegisterServer: CreateComponent registration - " << input << std::endl;

	logger << "DllRegisterServer: Success" << std::endl;
	return hResult;
}

STDAPI DllUnregisterServer()
{
	HRESULT hResult = NULL;
	logger << "DllUnregisterServer: " << "Start" << std::endl;

	// TODO: Unregister through iteration over array
	hResult = UnregisterServer(
		CLSID_CreateComponent,
		VINDX_CreateComponent,
		PGID_CreateComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllUnregisterServer: CreateComponent deregistration failed. HResult - " << hResult << std::endl;
		return hResult;
	}
	hResult = UnregisterServer(
		CLSID_StartComponent,
		VINDX_StartComponent,
		 PGID_StartComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllUnregisterServer: StartComponent deregistration failed. HResult - " << hResult << std::endl;
		return hResult;
	}
	hResult = UnregisterServer(
		CLSID_ClientComponent,
		VINDX_ClientComponent,
		 PGID_ClientComponent
	);
	if (FAILED(hResult))
	{
		logger << "DllUnregisterServer: ClientComponent deregistration failed. HResult - " << hResult << std::endl;
		return hResult;
	}

	logger << "DllUnregisterServer: Success" << std::endl;
	return hResult;
}

const int CLSID_STRING_SIZE = 39;

#define CHECK(b) if (b == FALSE) { return S_FALSE; }

HRESULT RegisterServer(HMODULE hModule,            // DLL module handle
	const CLSID& clsid,         // Class ID
	const WCHAR* szFriendlyName, // Friendly Name
	const WCHAR* szVerIndProgID, // Programmatic
	const WCHAR* szProgID)       //   IDs
{

	WCHAR szModule[512];
	DWORD dwResult = GetModuleFileName(hModule, szModule, sizeof(szModule) / sizeof(WCHAR));
	assert(dwResult != 0);

	WCHAR szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	WCHAR szKey[264];      //smw szKey[64]
	wcscpy_s(szKey, L"CLSID\\");
	wcscat_s(szKey, szCLSID);

	CHECK(setKeyAndValue(szKey, NULL, szFriendlyName));
	CHECK(setKeyAndValue(szKey, L"InprocServer32", szModule));
	CHECK(setKeyAndValue(szKey, L"ProgID", szProgID));
	CHECK(setKeyAndValue(szKey, L"VersionIndependentProgID", szVerIndProgID));
	CHECK(setKeyAndValue(szVerIndProgID, NULL, szFriendlyName));
	CHECK(setKeyAndValue(szVerIndProgID, L"CLSID", szCLSID));
	CHECK(setKeyAndValue(szVerIndProgID, L"CurVer", szProgID));
	CHECK(setKeyAndValue(szProgID, NULL, szFriendlyName));
	CHECK(setKeyAndValue(szProgID, L"CLSID", szCLSID));

	return S_OK;
}

HRESULT UnregisterServer(const CLSID& clsid,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID)

{
	WCHAR szCLSID[CLSID_STRING_SIZE];
	CLSIDtochar(clsid, szCLSID, sizeof(szCLSID));

	WCHAR szKey[64];
	wcscpy_s(szKey, L"CLSID\\");
	wcscat_s(szKey, szCLSID);

	LONG lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szKey);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szVerIndProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.
	lResult = recursiveDeleteKey(HKEY_CLASSES_ROOT, szProgID);
	assert((lResult == ERROR_SUCCESS) || (lResult == ERROR_FILE_NOT_FOUND)); // Subkey may not exist.

	return S_OK;
}


void CLSIDtochar(const CLSID& clsid,      // Convert a CLSID to a char string.   
	WCHAR* szCLSID,          // smw:char* szCLSID,
	int length)
{
	assert(length >= CLSID_STRING_SIZE);

	LPOLESTR wszCLSID = NULL;
	HRESULT hr = StringFromCLSID(clsid, &wszCLSID);
	assert(SUCCEEDED(hr));
	wcscpy_s(szCLSID, CLSID_STRING_SIZE, wszCLSID);
	CoTaskMemFree(wszCLSID);

}


LONG recursiveDeleteKey(HKEY hKeyParent,           // Parent of key to delete
	const WCHAR* lpszKeyChild)  // Key to delete
{

	HKEY hKeyChild;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyChild, 0,
		KEY_ALL_ACCESS, &hKeyChild);
	if (lRes != ERROR_SUCCESS)
	{
		return lRes;
	}

	FILETIME time;
	WCHAR szBuffer[256];
	DWORD dwSize = 256;
	while (RegEnumKeyEx(hKeyChild, 0, szBuffer, &dwSize, NULL,
		NULL, NULL, &time) == S_OK)
	{
		lRes = recursiveDeleteKey(hKeyChild, szBuffer);
		if (lRes != ERROR_SUCCESS)
		{
			RegCloseKey(hKeyChild);
			return lRes;
		}
		dwSize = 256;
	}

	RegCloseKey(hKeyChild);
	return RegDeleteKey(hKeyParent, lpszKeyChild);
}

BOOL setKeyAndValue(const WCHAR* szKey,         // smw:const char* szKey
	const WCHAR* szSubkey,      // smw:const char* szSubkey,
	const WCHAR* szValue)       // smw: const char* szValue
{
	HKEY hKey;
	WCHAR szKeyBuf[1024];

	wcscpy_s(szKeyBuf, szKey);

	if (szSubkey != NULL)
	{
		wcscat_s(szKeyBuf, L"\\");
		wcscat_s(szKeyBuf, szSubkey);
	}
	long lResult = RegCreateKeyEx(HKEY_CLASSES_ROOT,
		szKeyBuf,
		0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL,
		&hKey, NULL);
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (szValue != NULL)
	{
		RegSetValueEx(hKey, NULL, 0, REG_SZ,
			(BYTE*)szValue,
			2 * wcslen(szValue) + 1);
	}

	RegCloseKey(hKey);
	return TRUE;
}
