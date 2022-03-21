#pragma once
#include "pch.h"
#include "IAdder.h"
#include "IMultiplier.h"

// {F99EFC5E-6A88-483D-A2F4-94D7A95C0795}
static const GUID IID_C_OS12 =
{ 0xf99efc5e, 0x6a88, 0x483d, { 0xa2, 0xf4, 0x94, 0xd7, 0xa9, 0x5c, 0x7, 0x95 } };

#define FNAME	L"OS12.COM"
#define VINDX	L"OS12.1"
#define PGID	L"OS12"

extern long currentComponentsAmount;

class C_OS12 : public IAdder, public IMultiplier
{
public:
	C_OS12();
	~C_OS12();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE Add(const double x, const double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Sub(const double x, const double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Mul(const double x, const double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Div(const double x, const double y, double& z);

private:
	ULONG m_cRef;
	int id;
};
