#pragma once
#include "pch.h"
#include "Values.h"

// {0C0C240A-B673-492E-84DD-44A6B64CDCFC}
static const GUID IID_ICreateStorage =
{ 0xc0c240a, 0xb673, 0x492e, { 0x84, 0xdd, 0x44, 0xa6, 0xb6, 0x4c, 0xdc, 0xfc } };

interface ICreateStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE], const char HTUsersGroupName[FILEPATH_SIZE]) = 0;
};
