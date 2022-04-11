#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdio.h>

static WCHAR ServiceName[] = L"HarwexStorage";
static const char* TraceFileName = "D:\\UniveristyInput\\storage-service.log";
extern std::wofstream traceFile;
extern char* storagePath;
extern char* snapshotsDirectoryPath;
extern std::string loggerPath;

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

template<class ...Args>
void trace(Args... message)
{
	traceFile.open(TraceFileName, std::wofstream::app);
	(traceFile << ... << message) << "\n";
	traceFile.close();
}
