#pragma once
#include "pch.h"
#include "IOpenStorage.h"
#include "ICloseStorage.h"
#include "ICrud.h"
#include "IErrorHandler.h"
#include "IElementCreator.h"
#include "IElementPrinter.h"

// {970D29EA-E987-40A4-BC26-AC6EDF62BE4E}
static const GUID IID_IClientComponent =
{ 0x970d29ea, 0xe987, 0x40a4, { 0xbc, 0x26, 0xac, 0x6e, 0xdf, 0x62, 0xbe, 0x4e } };

interface IClientComponent: IOpenStorage, ICloseStorage, ICrud, IErrorHandler, IElementCreator, IElementPrinter
{

};
