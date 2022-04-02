#pragma once
#include "pch.h"
#include "ILoadStorage.h"
#include "ICloseStorage.h"
#include "ISnap.h"

// {E3DD01E3-80C3-4B7B-8E06-A72D36379043}
static const GUID IID_IStartComponent =
{ 0xe3dd01e3, 0x80c3, 0x4b7b, { 0x8e, 0x6, 0xa7, 0x2d, 0x36, 0x37, 0x90, 0x43 } };

interface IStartComponent : ILoadStorage, ICloseStorage, ISnap
{

};
