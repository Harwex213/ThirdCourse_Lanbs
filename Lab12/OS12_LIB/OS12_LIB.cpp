#include "pch.h"
#include "framework.h"
#include "OS12_LIB.h"

namespace OS12
{
	LPVOID Init()
	{
		IUnknown* pIUnknown = NULL;

		HRESULT hResult = CoInitialize(NULL);
		if (FAILED(hResult))
		{
			throw (int)hResult;
		}

		hResult = CoCreateInstance(IID_C_OS12, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
		if (FAILED(hResult))
		{
			throw (int)hResult;
		}

		return pIUnknown;
	}

	void Dispose(LPVOID h)
	{
		((IUnknown*)h)->Release();
		CoFreeUnusedLibraries();
	}

	namespace Adder
	{
		double Add(LPVOID h, double x, double y)
		{
			if (h == NULL)
			{
				throw 10000;
			}
			IUnknown* pIUnknown = (IUnknown*)h;
			IAdder* pIAdder = NULL;
			double z = 0.0;

			HRESULT hResult = pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			hResult = pIAdder->Add(x, y, z);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			pIAdder->Release();
			return z;
		}

		double Sub(LPVOID h, double x, double y)
		{
			IUnknown* pIUnknown = (IUnknown*)h;
			IAdder* pIAdder = NULL;
			double z = 0.0;

			HRESULT hResult = pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			hResult = pIAdder->Sub(x, y, z);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			pIAdder->Release();
			return z;
		}
	}

	namespace Multiplier
	{
		double Mul(LPVOID h, double x, double y)
		{
			if (h == NULL)
			{
				throw 10000;
			}
			IUnknown* pIUnknown = (IUnknown*)h;
			IMultiplier* pIMultiplier = NULL;
			double z = 0.0;

			HRESULT hResult = pIUnknown->QueryInterface(IID_IMultiplier, (void**)&pIMultiplier);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			hResult = pIMultiplier->Mul(x, y, z);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			pIMultiplier->Release();
			return z;
		}

		double Div(LPVOID h, double x, double y)
		{
			if (h == NULL)
			{
				throw 10000;
			}
			if (y == 0)
			{
				throw 10001;
			}
			IUnknown* pIUnknown = (IUnknown*)h;
			IMultiplier* pIMultiplier = NULL;
			double z = 0.0;

			HRESULT hResult = pIUnknown->QueryInterface(IID_IMultiplier, (void**)&pIMultiplier);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			hResult = pIMultiplier->Div(x, y, z);
			if (FAILED(hResult))
			{
				throw (int)hResult;
			}

			pIMultiplier->Release();
			return z;
		}

	}
}
