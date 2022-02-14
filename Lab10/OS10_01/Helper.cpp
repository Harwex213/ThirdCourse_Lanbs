#include "Helper.h"
#include "HtHandle.h"
#include "Element.h"
#include <string>
#include <ctime>

namespace HT
{
	DWORD CalcHashTableMaxSizeMemory(int htCapacity, int elMaxKeyLength, int elMaxPayloadLength)
	{
		DWORD elementSize = CalcElementMaxSizeMemory(elMaxKeyLength, elMaxPayloadLength);
		return sizeof(HTHANDLE) + htCapacity * (elementSize);
	}

	DWORD CalcElementMaxSizeMemory(int elMaxKeyLength, int elMaxPayloadLength)
	{
		return sizeof(Element) + elMaxKeyLength + elMaxPayloadLength;
	}

	int TruncateStrByMax(std::string& truncatedStr, const char* str, int strLength, int maxStrLength)
	{
		truncatedStr = str;
		int newStrLength = strLength > maxStrLength ? maxStrLength : strLength;
		truncatedStr.resize(newStrLength);
		truncatedStr.shrink_to_fit();

		return newStrLength;
	}

	std::string GetFileName(const char* filename)
	{
		std::string str(filename);
		std::size_t found = str.find_last_of("/\\");
		str = str.substr(found + 1);
		found = str.find_last_of(".");
		str = str.substr(0, found);
		return str;
	}

	std::string GetFilePath(const char* filename)
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
}