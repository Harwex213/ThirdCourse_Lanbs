#pragma once
#include "pch.h"

// {F5EE8084-17AA-45E0-85E4-E6CC024261B9}
static const GUID IID_ISnap =
{ 0xf5ee8084, 0x17aa, 0x45e0, { 0x85, 0xe4, 0xe6, 0xcc, 0x2, 0x42, 0x61, 0xb9 } };

interface ISnap : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE ExecuteSnap() = 0;
};
