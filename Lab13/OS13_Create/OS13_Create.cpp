#include <iostream>
#include <Windows.h>
#pragma comment(lib, "../x64/Debug/OS13_HTCOM_Static.lib")
#include "CreateComponentApi.h"

int main(int argc, char* argv[])
{
	ICreateComponent* pComponent = NULL;
	try
	{
		if (argc != 8) {
			throw std::exception("Wrong process arguments. Should be fileName(char*), loggerName(char*), secSnapshotInteval, capacity, maxKeyLength, maxPayloadLength, usersGroupName(char*)");
		}
		const char* fileName = argv[1];
		std::string loggerName = argv[2];
		const char* usersGroupName = argv[7];
		int params[4];
		for (int i = 0; i < 4; ++i) {
			params[i] = strtol(argv[i + 3], NULL, 10);
		}

		pComponent = CreateComponentApi::Init(loggerName);

		CreateComponentApi::CreateStorage(pComponent, params[1], params[0], params[2], params[3], fileName, usersGroupName);

		printf_s("HT-Storage Created: filePath = %s, loggerPath = %s, secSnapshotInterval = %d, capacity = %d, maxKeyLength = %d, maxPayloadLength = %d, usersGroupName = %s\n",
			fileName, loggerName.c_str(), params[0], params[1], params[2], params[3], usersGroupName);
	}
	catch (const std::exception& error)
	{
		printf_s("%s\n", error.what());
	}

	if (pComponent != NULL)
	{
		CreateComponentApi::Dispose(pComponent);
	}
	system("pause");
}
