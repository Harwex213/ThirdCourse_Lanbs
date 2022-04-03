#pragma once
#include "pch.h"

// {4B4EBD11-7B8B-467F-99AC-E6943C123DB7}
static const GUID IID_ILoadStorage =
{ 0x4b4ebd11, 0x7b8b, 0x467f, { 0x99, 0xac, 0xe6, 0x94, 0x3c, 0x12, 0x3d, 0xb7 } };

interface ILoadStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE LoadStorage(const char fileName[FILEPATH_SIZE], const char snapshotsDirectoryPath[FILEPATH_SIZE]) = 0;
};

