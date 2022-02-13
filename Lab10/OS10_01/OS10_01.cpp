#include <iostream>
#include <string>
#include <Windows.h>
#include "Element.h"
#include "HashTable.h"
#include "Api.h"
using namespace std;

HT::HTHANDLE* Create()
{
	HT::HTHANDLE* htHandle = HT::Create(10, 3, 20, 50, "../input/test.ht");
	if (htHandle)
	{
		cout << "htHandle created successfully\n";
		return htHandle;
	}

	throw "Error! htHandle wasn't created\n";
}

HT::HTHANDLE* Open()
{
	HT::HTHANDLE* htHandle = HT::Open("../input/test.ht");
	if (htHandle)
	{
		cout << "htHandle opened successfully\n";
		return htHandle;
	}

	throw "Error! htHandle wasn't opened\n";
}

void Insert(HT::HTHANDLE* htHandle, const char* key, const char* value)
{
	if (HT::Insert(htHandle, new HT::Element(key, strlen(key) + 1, value, strlen(value) + 1)))
	{
		printf_s("Element(%s) was created successfully\n", key);
		return;
	}

	string error = "Error! Element("; error += key; error += +") wasn't created";
	throw error.c_str();
}

void Delete(HT::HTHANDLE* htHandle, const char* key)
{
	if (HT::Delete(htHandle, new HT::Element(key, strlen(key) + 1)))
	{
		printf_s("Element(%s) was deleted successfully\n", key);
		return;
	}

	string error = "Error! Element("; error += key; error += +") wasn't deleted";
	throw error.c_str();
}

HT::Element* Get(HT::HTHANDLE* htHandle, const char* key)
{
	HT::Element* element = HT::Get(htHandle, new HT::Element(key, strlen(key) + 1));
	if (element)
	{
		printf_s("Element(%s) was found successfully\n", key);
	}
	else
	{
		printf_s("Element(%s) wasn't found\n", key);
	}
	return element;
}

void Close(HT::HTHANDLE* htHandle)
{
	if (HT::Close(htHandle))
	{
		cout << "htHandle closed successfully\n";
		return;
	}

	throw "Error! htHandle wasn't closed\n";
}

void StartTest()
{
	printf_s("\n\n--- Test One Start ---\n\n");
	HT::HTHANDLE* htHandle = Create();

	Insert(htHandle, "key1337", "ewqiequsf");

	HT::Element* element = Get(htHandle, "key1337");
	HT::Print(element);

	Delete(htHandle, "key1337");

	element = Get(htHandle, "key1337");
	HT::Print(element);

	Close(htHandle);
	printf_s("\n\n--- Test One End ---\n\n");
}

void Test02()
{
	printf_s("\n\n--- Test Three Start ---\n\n");
	HT::HTHANDLE* htHandle = Open();

	HT::Element* element;
	string key = "key";
	string payload = "payload";

	cout << endl;

	for (int i = 0; i < 9; i++)
	{
		key += std::to_string(i);
		payload += std::to_string(i);

		Insert(htHandle, key.c_str(), payload.c_str());

		key.resize(3);
		payload.resize(7);
	}

	cout << endl;

	for (int i = 0; i < 9; i++)
	{
		key += std::to_string(i);
		
		element = Get(htHandle, key.c_str());
		HT::Print(element);

		key.resize(3);
	}

	cout << endl;

	for (int i = 0; i < 5; i ++)
	{
		key += std::to_string(i);

		Delete(htHandle, key.c_str());

		key.resize(3);
	}

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	element = Get(htHandle, "key1337");
	HT::Print(element);

	cout << endl;

	cout << "Current size: " << htHandle->currentSize << endl;
	for (int i = 20; i < 29; i++)
	{
		key += std::to_string(i);
		payload += std::to_string(i);

		Insert(htHandle, key.c_str(), payload.c_str());

		key.resize(3);
		payload.resize(7);
	}
	cout << "Current size: " << htHandle->currentSize << endl;

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	for (int i = 5; i < 9; i++)
	{
		key += std::to_string(i);

		Delete(htHandle, key.c_str());

		key.resize(3);
	}

	for (int i = 20; i < 29; i++)
	{
		key += std::to_string(i);

		Delete(htHandle, key.c_str());

		key.resize(3);
	}

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	for (int i = 0; i < 3; i++)
	{
		key += std::to_string(i);
		payload += std::to_string(i);

		Insert(htHandle, key.c_str(), payload.c_str());

		key.resize(3);
		payload.resize(7);
	}

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	for (int i = 0; i < 3; i++)
	{
		key += std::to_string(i);

		Delete(htHandle, key.c_str());

		key.resize(3);
	}

	cout << endl;

	Close(htHandle);

	printf_s("\n\n--- Test Three End ---\n\n");
}


void Test03()
{
	printf_s("\n\n--- Test Two Start ---\n\n");
	HT::HTHANDLE* htHandle = Open();
	string key = "key";
	string payload = "payload";

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	for (int i = 0; i < 3; i++)
	{
		key += std::to_string(i);
		payload += std::to_string(i);

		Insert(htHandle, key.c_str(), payload.c_str());

		key.resize(3);
		payload.resize(7);
	}

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	for (int i = 0; i < 3; i++)
	{
		key += std::to_string(i);

		Delete(htHandle, key.c_str());

		key.resize(3);
	}
	HT::Print(element);

	cout << endl;

	HT::PrintAllElements(htHandle);

	cout << endl;

	Close(htHandle);
	printf_s("\n\n--- Test Two End ---\n\n");
}


int main()
{
	try
	{
		StartTest();
		//Test02();
		//Test03();

		printf_s("\n---Complete---\n");
	}
	catch (const char* error)
	{
		printf_s(error);
		return 1;
	}

	return 0;
}