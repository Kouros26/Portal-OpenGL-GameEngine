#pragma once
#include <fstream>
#include <filesystem>
#include <string>

#define DEBUG_LOG(message) \
{\
char buffer[4098]{0};\
snprintf(buffer, 4098, "%s(%i): %s\n", __FILE__, __LINE__, message);\
OutputDebugString(buffer);\
}

class Log
{
	static Log& log;
	std::fstream file;
	static Log& GetInstance();

	Log();
	~Log();	

	template<typename T>
	static const T arg(const T& value)
	{
		return value;
	}

	template<typename T>
	static const char* arg(const std::basic_string<T>& str)
	{
		return str.c_str();
	}

	template<typename ... T>
	static std::string StringFormat(const std::string str, const T& ... value)
	{
		int bufferSize = snprintf(nullptr, 0, str.c_str(), arg(value) ... );
		char* buffer = new char[bufferSize];
		snprintf(buffer, bufferSize + 1, str.c_str(), arg(value) ...);
		return buffer;
	}

public :
	static void OpenFile(const std::filesystem::path& filename);

	template<typename ... T>
	static void Print(const std::string& str, const T& ... value)
	{
		std::string newString = StringFormat(str, value ...);
		printf(newString.c_str());
		
		if (Log::log.file.is_open())
		{
			Log::log.file.write(newString.c_str(), newString.length());
			Log::log.file.flush();
		}
	}
};