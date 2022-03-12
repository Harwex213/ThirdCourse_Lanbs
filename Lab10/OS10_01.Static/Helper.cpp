#include "pch.h"
#include "Helper.h"
#include "HtHandle.h"
#include "Element.h"

namespace HT
{
	DWORD CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength)
	{
		DWORD elementSize = CalcElementMaxSizeMemory(elMaxKeyLength, elMaxPayloadLength);
		return sizeof(SharedMemory) + htCapacity * (elementSize);
	}

	DWORD CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength)
	{
		return sizeof(Element) + elMaxKeyLength + elMaxPayloadLength;
	}

	std::string TruncateStrByMax(int& strLength, const char* str, int maxStrLength)
	{
		strLength = strLength > maxStrLength ? maxStrLength : strLength;
		std::string truncatedStr = str;
		truncatedStr.resize(strLength);
		truncatedStr.shrink_to_fit();

		return truncatedStr;
	}

	std::string GetFileName(const char filename[CHAR_MAX_LENGTH])
	{
		std::string str(filename);
		std::size_t found = str.find_last_of("/\\");
		str = str.substr(found + 1);
		found = str.find_last_of(".");
		str = str.substr(0, found);
		return str;
	}

	std::string GetFilePath(const char filename[CHAR_MAX_LENGTH])
	{
		std::string str(filename);
		std::size_t found = str.find_last_of("/\\");
		str = str.substr(0, found);
		return str;
	}

	std::string TimeToLocalString(time_t time)
	{
		tm* tmLocal = new tm();
		localtime_s(tmLocal , &time);
		std::string output = std::to_string(tmLocal->tm_yday) + "-" + std::to_string(tmLocal->tm_hour) + "-" +
			std::to_string(tmLocal->tm_min) + "-" + std::to_string(tmLocal->tm_sec);
		delete tmLocal;
		return output;
	}

	std::string GenerateRandomString(int length)
	{
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

		std::random_device rd;
		std::mt19937 generator(rd());

		std::shuffle(str.begin(), str.end(), generator);

		return str.substr(0, length);     
	}
}