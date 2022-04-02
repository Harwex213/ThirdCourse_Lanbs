#pragma once
#include "pch.h"

// {15EA397A-FE29-4380-BD86-CD106C100A52}
static const GUID IID_IErrorHandler =
{ 0x15ea397a, 0xfe29, 0x4380, { 0xbd, 0x86, 0xcd, 0x10, 0x6c, 0x10, 0xa, 0x52 } };

interface IErrorHandler : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE GetLastError(char* error) = 0;
};
