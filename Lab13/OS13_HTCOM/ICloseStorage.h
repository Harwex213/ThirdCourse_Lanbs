#pragma once
#include "pch.h"

// {B79F21B4-34FF-48C7-A6FC-7A9CB1771E83}
static const GUID IID_ICloseStorage =
{ 0xb79f21b4, 0x34ff, 0x48c7, { 0xa6, 0xfc, 0x7a, 0x9c, 0xb1, 0x77, 0x1e, 0x83 } };

interface ICloseStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CloseStorage() = 0;
};

