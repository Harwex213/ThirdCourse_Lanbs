#pragma once
#include <objbase.h>

#define FILEPATH_SIZE 512

// {B79F21B4-34FF-48C7-A6FC-7A9CB1771E83}
static const GUID IID_ICloseStorage =
{ 0xb79f21b4, 0x34ff, 0x48c7, { 0xa6, 0xfc, 0x7a, 0x9c, 0xb1, 0x77, 0x1e, 0x83 } };

interface ICloseStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CloseStorage() = 0;
};

// {0C0C240A-B673-492E-84DD-44A6B64CDCFC}
static const GUID IID_ICreateStorage =
{ 0xc0c240a, 0xb673, 0x492e, { 0x84, 0xdd, 0x44, 0xa6, 0xb6, 0x4c, 0xdc, 0xfc } };

interface ICreateStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE]) = 0;
};

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

// {4B4EBD11-7B8B-467F-99AC-E6943C123DB7}
static const GUID IID_ILoadStorage =
{ 0x4b4ebd11, 0x7b8b, 0x467f, { 0x99, 0xac, 0xe6, 0x94, 0x3c, 0x12, 0x3d, 0xb7 } };

interface ILoadStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE LoadStorage() = 0;
};

// {5A8DD7BD-75D2-448B-A8E6-CEDE5CD60C12}
static const GUID IID_IOpenStorage =
{ 0x5a8dd7bd, 0x75d2, 0x448b, { 0xa8, 0xe6, 0xce, 0xde, 0x5c, 0xd6, 0xc, 0x12 } };

interface IOpenStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE OpenStorage() = 0;
};

// {F5EE8084-17AA-45E0-85E4-E6CC024261B9}
static const GUID IID_ISnap =
{ 0xf5ee8084, 0x17aa, 0x45e0, { 0x85, 0xe4, 0xe6, 0xcc, 0x2, 0x42, 0x61, 0xb9 } };

interface ISnap : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE ExecuteSnap() = 0;
};

// {6480342D-3315-444A-A6FF-F5CEB435A20E}
static const GUID IID_ICreateComponent =
{ 0x6480342d, 0x3315, 0x444a, { 0xa6, 0xff, 0xf5, 0xce, 0xb4, 0x35, 0xa2, 0xe } };

interface ICreateComponent : ICreateStorage
{

};

// {E3DD01E3-80C3-4B7B-8E06-A72D36379043}
static const GUID IID_IStartComponent =
{ 0xe3dd01e3, 0x80c3, 0x4b7b, { 0x8e, 0x6, 0xa7, 0x2d, 0x36, 0x37, 0x90, 0x43 } };

interface IStartComponent : ILoadStorage, ICloseStorage, ISnap
{

};

// {970D29EA-E987-40A4-BC26-AC6EDF62BE4E}
static const GUID IID_IClientComponent =
{ 0x970d29ea, 0xe987, 0x40a4, { 0xbc, 0x26, 0xac, 0x6e, 0xdf, 0x62, 0xbe, 0x4e } };

interface IClientComponent : IOpenStorage, ICloseStorage, ICrud
{

};

// {00DB7A85-8321-4530-8D2A-FD4AA3492585}
static const GUID CLSID_CreateComponent =
{ 0xdb7a85, 0x8321, 0x4530, { 0x8d, 0x2a, 0xfd, 0x4a, 0xa3, 0x49, 0x25, 0x85 } };

// {E26D3691-759D-427E-B3D3-697DEF7AA33C}
static const GUID CLSID_StartComponent =
{ 0xe26d3691, 0x759d, 0x427e, { 0xb3, 0xd3, 0x69, 0x7d, 0xef, 0x7a, 0xa3, 0x3c } };

// {C9DCCD71-3627-41B2-A434-343C5E661089}
static const GUID CLSID_ClientComponent =
{ 0xc9dccd71, 0x3627, 0x41b2, { 0xa4, 0x34, 0x34, 0x3c, 0x5e, 0x66, 0x10, 0x89 } };
