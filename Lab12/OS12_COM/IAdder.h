#pragma once
#include <objbase.h>

// {14A14317-24FF-4AE8-BA4B-5883134DDDF7}
static const GUID IID_IAdder =
{ 0x14a14317, 0x24ff, 0x4ae8, { 0xba, 0x4b, 0x58, 0x83, 0x13, 0x4d, 0xdd, 0xf7 } };
interface IAdder :IUnknown
{
	virtual HRESULT __stdcall Add(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z) = 0;
};
