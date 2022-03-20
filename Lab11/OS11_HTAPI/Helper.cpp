#include "pch.h"
#include "Helper.h"

namespace Helper
{
	std::string truncateString(const char* str, int& strLength, int threshold)
	{
		std::string truncatedStr = str;
		strLength = strLength > threshold ? threshold : strLength;

		truncatedStr.resize(strLength - 1);
		truncatedStr.shrink_to_fit();
		return truncatedStr;
	}

	std::string generateRandomString(int length)
	{
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

		std::random_device rd;
		std::mt19937 generator(rd());

		std::shuffle(str.begin(), str.end(), generator);

		return str.substr(0, length);
	}

	std::string parseFileName(const char* filePath)
	{
		std::string str(filePath);
		std::size_t found = str.find_last_of("/\\");
		str = str.substr(found + 1);
		found = str.find_last_of(".");
		str = str.substr(0, found);
		return str;
	}

	std::string parseFilePath(const char* filePath)
	{
		std::string str(filePath);
		std::size_t found = str.find_last_of("/\\");
		str = str.substr(0, found);
		return str;

	}

	std::string timeToLocalString(time_t time)
	{
		tm* tmLocal = new tm();
		localtime_s(tmLocal, &time);
		std::string output = std::to_string(tmLocal->tm_yday) + "-" + std::to_string(tmLocal->tm_hour) + "-" +
			std::to_string(tmLocal->tm_min) + "-" + std::to_string(tmLocal->tm_sec);
		delete tmLocal;
		return output;
	}

}