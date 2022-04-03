#pragma once
#include "pch.h"

// {5A8DD7BD-75D2-448B-A8E6-CEDE5CD60C12}
static const GUID IID_IOpenStorage =
{ 0x5a8dd7bd, 0x75d2, 0x448b, { 0xa8, 0xe6, 0xce, 0xde, 0x5c, 0xd6, 0xc, 0x12 } };

interface IOpenStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE OpenStorage() = 0;
};
