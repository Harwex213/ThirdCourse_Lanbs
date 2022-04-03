#pragma once
#include "pch.h"
#include "Element.h"

// {3BDB3471-7C5E-46A1-B651-999567D4DD04}
static const GUID IID_IElementPrinter =
{ 0x3bdb3471, 0x7c5e, 0x46a1, { 0xb6, 0x51, 0x99, 0x95, 0x67, 0xd4, 0xdd, 0x4 } };

interface IElementPrinter : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE PrintElement(Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE PrintAllElements() = 0;
};
