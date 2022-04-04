#pragma once
#include "pch.h"
#include "Element.h"

// {EB67EBBF-66C3-4E00-A346-0F5888A3834F}
static const GUID IID_ICrud =
{ 0xeb67ebbf, 0x66c3, 0x4e00, { 0xa3, 0x46, 0xf, 0x58, 0x88, 0xa3, 0x83, 0x4f } };

interface ICrud : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Find(Element*& element) = 0;
	virtual HRESULT STDMETHODCALLTYPE Insert(Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE Update(Element* element, const void* newPayload, int newPayloadLength) = 0;
	virtual HRESULT STDMETHODCALLTYPE Delete(Element* element) = 0;
};
