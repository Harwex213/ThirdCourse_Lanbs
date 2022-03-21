#include <objbase.h>
#include <iostream>

// {14A14317-24FF-4AE8-BA4B-5883134DDDF7}
static const GUID IID_IAdder =
{ 0x14a14317, 0x24ff, 0x4ae8, { 0xba, 0x4b, 0x58, 0x83, 0x13, 0x4d, 0xdd, 0xf7 } };
interface IAdder :IUnknown
{
	virtual HRESULT __stdcall Add(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z) = 0;
};

// {661B265C-8A5A-4F54-8C46-38439999EA5A}
static const GUID IID_IMultiplier =
{ 0x661b265c, 0x8a5a, 0x4f54, { 0x8c, 0x46, 0x38, 0x43, 0x99, 0x99, 0xea, 0x5a } };
interface IMultiplier :IUnknown
{
	virtual HRESULT __stdcall Mul(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Div(const double x, const double y, double& z) = 0;
};

static const GUID IID_C_OS12 =
{ 0xf99efc5e, 0x6a88, 0x483d, { 0xa2, 0xf4, 0x94, 0xd7, 0xa9, 0x5c, 0x7, 0x95 } };

#define IERR(s)    std::cout<<"error "<<s<<std::endl
#define IRES(s,r)  std::cout<<s<<r<<std::endl


int main()
{
	IUnknown* pIUnknown = NULL;
	IAdder* pIAdder = NULL;
	IMultiplier* pMultiplier = NULL;

	CoInitialize(NULL);                        // инициализация библиотеки OLE32
	HRESULT hr0 = CoCreateInstance(IID_C_OS12, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr0))
	{
		std::cout << "CoCreateInstance succeeded" << std::endl;
		if (SUCCEEDED(pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder)))
		{
			{
				double z = 0.0;
				if (!SUCCEEDED(pIAdder->Add(2.0, 3.0, z)))  IERR("IAdder::Add");
				else IRES("IAdder::Add = ", z);
			}
			{
				double z = 0.0;
				if (!SUCCEEDED(pIAdder->Sub(2.0, 3.0, z)))  IERR("IAdder::Sub");
				else IRES("IAdder::Sub = ", z);
			}
			if (SUCCEEDED(pIAdder->QueryInterface(IID_IMultiplier, (void**)&pMultiplier)))
			{
				{
					double z = 0.0;
					if (!SUCCEEDED(pMultiplier->Mul(2.0, 3.0, z))) IERR("IMultiplier::Mul");
					else IRES("Multiplier::Mul = ", z);
				}
				{
					double z = 0.0;
					if (!SUCCEEDED(pMultiplier->Div(2.0, 3.0, z))) IERR("IMultiplier::Div");
					else IRES("IMultiplier::Div = ", z);
				}
				if (SUCCEEDED(pMultiplier->QueryInterface(IID_IAdder, (void**)&pIAdder)))
				{
					double z = 0.0;
					if (!SUCCEEDED(pIAdder->Add(2.0, 3.0, z))) IERR("IAdder::Add");
					else IRES("IAdder::Add = ", z);
					pIAdder->Release();
				}
				else  IERR("IMultiplier->IAdder");
				pMultiplier->Release();
			}
			else IERR("IAdder->IMultiplier");
			pIAdder->Release();
		}
		else  IERR("IAdder");
	}
	else {
		std::cout << "CoCreateInstance error" << std::endl;
		pIUnknown = NULL;
	}
	if (pIUnknown != NULL)
	{
		pIUnknown->Release();
		CoFreeUnusedLibraries();                   // завершение работы с библиотекой    
	}  

	return 0;
}
