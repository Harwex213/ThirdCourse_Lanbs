#include <iostream>
#include <Windows.h>
#include "Element.h"
#include "HashTable.h"
#include "Api.h"
using namespace std;

struct element
{
	const void* key;
	const void* value;

	element()
	{

	}

	element(const char* pKey, const char* pValue, LPVOID elAddrBegin)
	{
		key = new((char*)elAddrBegin + sizeof(element))char[50];
		value = new((char*)elAddrBegin + sizeof(element) + 50)char[50];
		strcpy_s((char*)key, strlen(pKey) + 1, pKey);
		strcpy_s((char*)value, strlen(pValue) + 1, pValue);
	}
};

struct table
{
	int someNumber = 1919;
	int someNumber2 = 218;
};

int main()
{
	//HT::Element* element = new HT::Element("superKey", 9, "ahhhhhhhhhh", 2);
	//cout << element->getKey() << endl;

	//int megaTableSize = sizeof(table) + 10 * (sizeof(element) + 5 + 7);

	HANDLE hFile = CreateFileA(
		(LPCSTR) "../input/test.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Ti loh\n";
		return 1;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	HANDLE hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 1024, 0);
	if (hMapFile == NULL)
	{
		cout << "Ti mapping loh\n";
		return 2;
	}

	LPVOID addr = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (addr == NULL)
	{
		cout << "Ti mapView loh\n";
		return 3;
	}

	//HT::HTHANDLE* htHandle = HT::Create();

	//int capacity = 10;
	table* superTable = new(addr) table();
	//LPVOID elementsBeginning = (char*)addr + sizeof(table);
	//DWORD elementSize = sizeof(element) + 100;
	//element* element1 = new(elementsBeginning)element("key", "value", elementsBeginning);

	//table* superTable = (table*)addr;
	//LPVOID elementsBeginning = (char*)addr + sizeof(table);
	//DWORD elementSize = sizeof(element) + 100;
	//element* element1 = (element*)elementsBeginning;
	//element1->key = (char*)elementsBeginning + sizeof(element);
	//element1->value = (char*)elementsBeginning + sizeof(element) + 50;

	//element* element2 = new((char*)elementsBeginning + elementSize)element("key2", "value2", (char*)elementsBeginning + elementSize);

	cout << "complete" << endl;

	HT::Element element;
	element.getKey();


	return 0;
}