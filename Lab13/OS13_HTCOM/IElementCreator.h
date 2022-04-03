#pragma once
#include "pch.h"
#include "Element.h"

// {EA2D516C-6B57-4BD8-BF0F-684864EBAE78}
static const GUID IID_IElementCreator =
{ 0xea2d516c, 0x6b57, 0x4bd8, { 0xbf, 0xf, 0x68, 0x48, 0x64, 0xeb, 0xae, 0x78 } };

interface IElementCreator : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKey(Element*& element, const void* key, int keyLength) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKeyPayload(Element*& element, const void* key, int keyLength, const void* payload, int payloadLength) = 0;
};
