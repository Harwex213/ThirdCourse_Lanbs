#pragma once
#include "pch.h"

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

namespace OS12
{
	LPVOID Init();                                // инициализация OS12
	//   if CoCreateInstance(... IID_Unknown)!= succesfull --> throw (int)HRESULT  
	namespace Adder
	{
		double Add(LPVOID h, double x, double y);        // return x+y
		//  if QueryInteface(IID_IAdder) != succesfull -->  throw (int)HRESULT     
		double Sub(LPVOID h, double x, double y);        // return x-y
		//  if QueryInteface(IID_IAdder) != succesfull -->  throw (int)HRESULT
	}
	namespace Multiplier
	{
		double Mul(LPVOID h, double x, double y);        // return x*y
		//  if QueryInteface(IID_IMultiplier) != succesfull -->  throw (int)HRESULT 
		double Div(LPVOID h, double x, double y);        // return x/y
		//  if QueryInteface(IID_IMultiplier) != succesfull -->  throw (int)HRESULT 
	}
	void Dispose(LPVOID h);                       // завершение работы с OS12                  
}
