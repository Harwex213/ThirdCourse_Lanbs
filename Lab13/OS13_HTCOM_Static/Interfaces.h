#pragma once
#include "pch.h"
#include "Models.h"

#define FILEPATH_SIZE 512
#define USER_NAME_SIZE 256

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
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(int capacity, int secSnapshotInterval, int   maxKeyLength, int   maxPayloadLength, const char fileName[FILEPATH_SIZE], const char HTUsersGroupName[FILEPATH_SIZE]) = 0;
};

// {EB67EBBF-66C3-4E00-A346-0F5888A3834F}
static const GUID IID_ICrud =
{ 0xeb67ebbf, 0x66c3, 0x4e00, { 0xa3, 0x46, 0xf, 0x58, 0x88, 0xa3, 0x83, 0x4f } };

interface ICrud : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE Find(Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE Insert(Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE Update(Element* element, const void* newPayload, int newPayloadLength) = 0;
	virtual HRESULT STDMETHODCALLTYPE Delete(Element* element) = 0;
};

// {EA2D516C-6B57-4BD8-BF0F-684864EBAE78}
static const GUID IID_IElementCreator =
{ 0xea2d516c, 0x6b57, 0x4bd8, { 0xbf, 0xf, 0x68, 0x48, 0x64, 0xeb, 0xae, 0x78 } };

interface IElementCreator : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKey(Element*& element, const void* key, int keyLength) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateElementWithKeyPayload(Element*& element, const void* key, int keyLength, const void* payload, int payloadLength) = 0;
};

// {3BDB3471-7C5E-46A1-B651-999567D4DD04}
static const GUID IID_IElementPrinter =
{ 0x3bdb3471, 0x7c5e, 0x46a1, { 0xb6, 0x51, 0x99, 0x95, 0x67, 0xd4, 0xdd, 0x4 } };

interface IElementPrinter : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE PrintElement(Element* element) = 0;
	virtual HRESULT STDMETHODCALLTYPE PrintAllElements() = 0;
};

// {15EA397A-FE29-4380-BD86-CD106C100A52}
static const GUID IID_IErrorHandler =
{ 0x15ea397a, 0xfe29, 0x4380, { 0xbd, 0x86, 0xcd, 0x10, 0x6c, 0x10, 0xa, 0x52 } };

interface IErrorHandler : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE GetLastError(char* error) = 0;
};

// {4B4EBD11-7B8B-467F-99AC-E6943C123DB7}
static const GUID IID_ILoadStorage =
{ 0x4b4ebd11, 0x7b8b, 0x467f, { 0x99, 0xac, 0xe6, 0x94, 0x3c, 0x12, 0x3d, 0xb7 } };

interface ILoadStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE LoadStorage(const char fileName[FILEPATH_SIZE], const char snapshotsDirectoryPath[FILEPATH_SIZE], const char user[USER_NAME_SIZE], const char password[USER_NAME_SIZE]) = 0;
	virtual HRESULT STDMETHODCALLTYPE LoadStorage(const char fileName[FILEPATH_SIZE], const char snapshotsDirectoryPath[FILEPATH_SIZE]) = 0;
};

// {5A8DD7BD-75D2-448B-A8E6-CEDE5CD60C12}
static const GUID IID_IOpenStorage =
{ 0x5a8dd7bd, 0x75d2, 0x448b, { 0xa8, 0xe6, 0xce, 0xde, 0x5c, 0xd6, 0xc, 0x12 } };

interface IOpenStorage : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const char filePath[FILEPATH_SIZE]) = 0;
};

interface ISnap : IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE ExecuteSnap() = 0;
};

// {970D29EA-E987-40A4-BC26-AC6EDF62BE4E}
static const GUID IID_IClientComponent =
{ 0x970d29ea, 0xe987, 0x40a4, { 0xbc, 0x26, 0xac, 0x6e, 0xdf, 0x62, 0xbe, 0x4e } };

interface IClientComponent : IOpenStorage, ICloseStorage, ICrud, IErrorHandler, IElementCreator, IElementPrinter
{
	virtual HRESULT STDMETHODCALLTYPE GetIsStorageClosed() = 0;
};

// {6480342D-3315-444A-A6FF-F5CEB435A20E}
static const GUID IID_ICreateComponent =
{ 0x6480342d, 0x3315, 0x444a, { 0xa6, 0xff, 0xf5, 0xce, 0xb4, 0x35, 0xa2, 0xe } };

interface ICreateComponent : ICreateStorage, IErrorHandler
{

};

// {E3DD01E3-80C3-4B7B-8E06-A72D36379043}
static const GUID IID_IStartComponent =
{ 0xe3dd01e3, 0x80c3, 0x4b7b, { 0x8e, 0x6, 0xa7, 0x2d, 0x36, 0x37, 0x90, 0x43 } };

interface IStartComponent : ILoadStorage, ICloseStorage, ISnap, IErrorHandler
{
	virtual HRESULT STDMETHODCALLTYPE GetStorageConfig(StorageConfig*& storageConfig) = 0;
};

// {C9DCCD71-3627-41B2-A434-343C5E661089}
static const GUID CLSID_ClientComponent =
{ 0xc9dccd71, 0x3627, 0x41b2, { 0xa4, 0x34, 0x34, 0x3c, 0x5e, 0x66, 0x10, 0x89 } };

// {00DB7A85-8321-4530-8D2A-FD4AA3492585}
static const GUID CLSID_CreateComponent =
{ 0xdb7a85, 0x8321, 0x4530, { 0x8d, 0x2a, 0xfd, 0x4a, 0xa3, 0x49, 0x25, 0x85 } };

// {E26D3691-759D-427E-B3D3-697DEF7AA33C}
static const GUID CLSID_StartComponent =
{ 0xe26d3691, 0x759d, 0x427e, { 0xb3, 0xd3, 0x69, 0x7d, 0xef, 0x7a, 0xa3, 0x3c } };
