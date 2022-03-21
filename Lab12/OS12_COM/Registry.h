#pragma once
#include "pch.h"

BOOL setKeyAndValue(const WCHAR* szKey,
	const WCHAR* szSubkey,
	const WCHAR* szValue);

void CLSIDtochar(const CLSID& clsid,
	WCHAR* szCLSID,
	int length);

LONG recursiveDeleteKey(HKEY hKeyParent, const WCHAR* szKeyChild);

HRESULT RegisterServer(HMODULE hModule,            // DLL module handle
	const CLSID& clsid,         // Class ID
	const WCHAR* szFriendlyName, // Friendly Name
	const WCHAR* szVerIndProgID, // Programmatic
	const WCHAR* szProgID);       //   IDs

HRESULT UnregisterServer(const CLSID& clsid,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);


