#include <iostream>
#include <string>

#include "Api.h"
#include "HtHandle.h"
using namespace std;

HT::HTHANDLE* Create()
{
	HT::HTHANDLE* htHandle = HT::Create(10, 3, 20, 50, "../input/test.ht");
	if (htHandle)
	{
		printf_s("THREAD %d: htHandle created successfully\n", GetCurrentThreadId());
		return htHandle;
	}

	throw "Error! htHandle wasn't created\n";
}

HT::HTHANDLE* Open()
{
	HT::HTHANDLE* htHandle = HT::Open("../input/test.ht");
	if (htHandle)
	{
		printf_s("THREAD %d: htHandle opened successfully\n", GetCurrentThreadId());
		return htHandle;
	}

	throw "Error! htHandle wasn't opened\n";
}

void Insert(HT::HTHANDLE* htHandle, const char* key, const char* value)
{
	if (HT::Insert(htHandle, new HT::Element(key, strlen(key) + 1, value, strlen(value) + 1)))
	{
		printf_s("THREAD %d: Element(%s) was created successfully\n", GetCurrentThreadId(), key);
		return;
	}

	printf_s("THREAD %d: Element(%s) wasn't created\n", GetCurrentThreadId(), key);
}

void Delete(HT::HTHANDLE* htHandle, const char* key)
{
	if (HT::Delete(htHandle, new HT::Element(key, strlen(key) + 1)))
	{
		printf_s("THREAD %d: Element(%s) was deleted successfully\n", GetCurrentThreadId(), key);
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
		printf_s("THREAD %d: Element(%s) was found successfully\n", GetCurrentThreadId(), key);
	}
	else
	{
		printf_s("THREAD %d: Element(%s) wasn't found\n", GetCurrentThreadId(), key);
	}
	return element;
}

void CloseHandle(HT::HTHANDLE* htHandle)
{
	if (HT::Close(htHandle))
	{
		printf_s("THREAD %d: htHandle closed successfully\n", GetCurrentThreadId());
		return;
	}

	throw "Error! htHandle wasn't closed\n";
}

int main()
{
	HT::HTHANDLE* htHandle = NULL;
	try
	{
		printf_s("\n\n--- Test Two Start on Thread %d ---\n\n", GetCurrentThreadId());
		htHandle = Open();
		string key = "key";
		string payload = "payload";

		HT::PrintAllElements(htHandle);

		//for (int i = 1; i < 3; i++)
		//{
		//	key += std::to_string(i);
		//	payload += std::to_string(i);

		//	Insert(htHandle, key.c_str(), payload.c_str());

		//	key.resize(3);
		//	payload.resize(7);
		//}

		CloseHandle(htHandle);
		printf_s("\n\n--- Test Two End, Thread %d ---\n\n", GetCurrentThreadId());

		printf_s("\n---Complete---\n");
	}
	catch (const char* error)
	{
		printf_s("Error: %s", HT::GetHTLastError(htHandle));
		return 1;
	}
    std::cout << "Hello World!\n";
}
