#include "pch.h"
#include "C_OS12.h"

long currentComponentsAmount = 0;

C_OS12::C_OS12() : m_cRef(1)
{
	InterlockedIncrement(&currentComponentsAmount);
	id = rand() % 10;

	LOG("C_OS12, Constructor call. currentComponentsAmount: ", currentComponentsAmount);
}

C_OS12::~C_OS12()
{
	InterlockedDecrement(&currentComponentsAmount);
	
	LOG("C_OS12, Destructor call. currentComponentsAmount: ", currentComponentsAmount);
}

HRESULT __stdcall C_OS12::QueryInterface(REFIID iid, void** ppv)
{
	;

	if (iid == IID_IUnknown || iid == IID_IAdder) {
		*ppv = static_cast<IAdder*>(this);
	}
	else if (iid == IID_IMultiplier) {
		*ppv = static_cast<IMultiplier*>(this);
	}
	else {
		ppv = NULL;
		LOG("C_OS12. QueryInterface - ", "E_NOINTERFACE");
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	LOG("C_OS12. QueryInterface - ", "S_OK");
	return S_OK;
}

ULONG __stdcall C_OS12::AddRef()
{
	;
	InterlockedIncrement(&m_cRef);
	LOG("C_OS12. AddRef: ", m_cRef);
	return m_cRef;
}

ULONG __stdcall C_OS12::Release()
{
	;
	if (InterlockedDecrement(&m_cRef) == 0) {
		LOG("C_OS12. Full release, id -  ", id);
		delete this;
		return 0;
	}
	LOG("C_OS12. Release: ", m_cRef);
	return m_cRef;
}

HRESULT __stdcall C_OS12::Add(const double x, const double y, double& z)
{
	;
	z = x + y;

	LOG("C_OS12. Add: ", z);
	return S_OK;
}

HRESULT __stdcall C_OS12::Sub(const double x, const double y, double& z)
{
	;
	z = x - y;

	LOG("C_OS12. Sub: ", z);
	return S_OK;
}

HRESULT __stdcall C_OS12::Mul(const double x, const double y, double& z)
{
	;
	z = x * y;

	LOG("C_OS12. Mul: ", z);
	return S_OK;
}

HRESULT __stdcall C_OS12::Div(const double x, const double y, double& z)
{
	;
	if (y == 0) {
		z = 0;

		LOG("C_OS12. Div: ", z);
		return S_FALSE;
	}
	z = x / y;

	LOG("C_OS12. Div: ", z);
	return S_OK;
}
