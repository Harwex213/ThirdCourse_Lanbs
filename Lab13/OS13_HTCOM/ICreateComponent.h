#pragma once
#include "pch.h"
#include "ICreateStorage.h"

// {6480342D-3315-444A-A6FF-F5CEB435A20E}
static const GUID IID_ICreateComponent =
{ 0x6480342d, 0x3315, 0x444a, { 0xa6, 0xff, 0xf5, 0xce, 0xb4, 0x35, 0xa2, 0xe } };

interface ICreateComponent : ICreateStorage
{

};
