#pragma once
#include "pch.h"

STDAPI DllCanUnloadNow();

STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv);