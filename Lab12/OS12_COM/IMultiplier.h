#pragma once
#include <objbase.h>

// {661B265C-8A5A-4F54-8C46-38439999EA5A}
static const GUID IID_IMultiplier =
{ 0x661b265c, 0x8a5a, 0x4f54, { 0x8c, 0x46, 0x38, 0x43, 0x99, 0x99, 0xea, 0x5a } };
interface IMultiplier :IUnknown
{
	virtual HRESULT __stdcall Mul(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Div(const double x, const double y, double& z) = 0;
};
