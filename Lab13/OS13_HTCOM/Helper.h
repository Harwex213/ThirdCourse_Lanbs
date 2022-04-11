#pragma once
#include "pch.h"

namespace Helper
{
	std::string truncateString(const char* str, int& strLength, int threshold);
	std::string generateRandomString(int length);
	std::string parseFileName(const char* filePath);
	std::string parseFilePath(const char* filePath);
	std::string timeToLocalString(time_t time);
	SECURITY_ATTRIBUTES getSecurityAttributes();
}