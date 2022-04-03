#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2) {
			throw std::exception("Wrong process arguments. Should be storagePath(char*)");
		}
		const char* storagePath = argv[1];
		std::string closeEventName = storagePath; closeEventName += "-closeEvent";
		HANDLE hCloseEvent = OpenEventA(EVENT_ALL_ACCESS, FALSE, closeEventName.c_str());
		if (hCloseEvent == NULL)
		{
			throw std::exception("Cannot open event to notify to close start");
		}

		SetEvent(hCloseEvent);
		Sleep(100);
		ResetEvent(hCloseEvent);

		printf_s("Success.\n");
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}

	system("pause");
}
