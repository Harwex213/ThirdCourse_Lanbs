#include <iostream>
#include <Windows.h>
#include "Element.h"
#include "HashTable.h"
#include "Api.h"
using namespace std;

void TestOne()
{
	HT::HTHANDLE* htHandle = HT::Create(15, 3, 20, 50, "../input/test.ht");
	if (htHandle)
	{
		cout << "htHandle created successfully\n";
	}
	else
	{
		cout << "htHandle created with errors\n";
		return;
	}

	if (HT::Insert(htHandle, new HT::Element("key1337", 8, "payload", 8)))
	{
		cout << "Element created successfully\n";
	}
	else
	{
		cout << "Element created with errors\n";
		return;
	}

	HT::Element* element = HT::Get(htHandle, new HT::Element("key1337", 8));
	if (element)
	{
		cout << "Element was got successfully\n";
	}
	else
	{
		cout << "Element was got with errors\n";
		return;
	}

	HT::Print(element);

	if (HT::Close(htHandle))
	{
		cout << "htHandle closed successfully\n";
	}
	else
	{
		cout << "htHandle closed with errors\n";
		return;
	}
}

void TestTwo()
{
	HT::HTHANDLE* htHandle = HT::Open("../input/test.ht");
	if (htHandle)
	{
		cout << "htHandle opened successfully\n";
	}
	else
	{
		cout << "htHandle opened with errors\n";
		return;
	}

	HT::Element* element = HT::Get(htHandle, new HT::Element("key1337", 8));
	if (element)
	{
		cout << "Element was got successfully\n";
	}
	else
	{
		cout << "Element was got with errors\n";
		return;
	}
	HT::Print(element);

	if (HT::Insert(htHandle, new HT::Element("key1338", 8, "payload12", 10)))
	{
		cout << "Element created successfully\n";
	}
	else
	{
		cout << "Element created with errors\n";
		return;
	}

	element = HT::Get(htHandle, new HT::Element("key1338", 8));
	if (element)
	{
		cout << "Element was got successfully\n";
	}
	else
	{
		cout << "Element was got with errors\n";
		return;
	}
	HT::Print(element);

	if (HT::Close(htHandle))
	{
		cout << "htHandle closed successfully\n";
	}
	else
	{
		cout << "htHandle closed with errors\n";
		return;
	}
}

int main()
{
	TestOne();
	TestTwo();

	cout << "complete" << endl;
	return 0;
}