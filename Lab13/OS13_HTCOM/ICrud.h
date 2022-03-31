#pragma once
#include "pch.h"

// {EB67EBBF-66C3-4E00-A346-0F5888A3834F}
static const GUID IID_ICrud =
{ 0xeb67ebbf, 0x66c3, 0x4e00, { 0xa3, 0x46, 0xf, 0x58, 0x88, 0xa3, 0x83, 0x4f } };

interface ICrud : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Find() = 0;
	virtual HRESULT STDMETHODCALLTYPE Insert() = 0;
	virtual HRESULT STDMETHODCALLTYPE Update() = 0;
	virtual HRESULT STDMETHODCALLTYPE Delete() = 0;
};
