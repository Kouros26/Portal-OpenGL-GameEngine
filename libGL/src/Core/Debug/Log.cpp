#include "../../../include/Core/Debug/Log.h"

Log& Log::log = Log::Log();

Log& Log::GetInstance()
{
	return log;
}

Log::Log()
{

}

Log::~Log()
{
	Log::log.file.close();
}

void Log::OpenFile(const std::filesystem::path& filename)
{
	Log::log.file.close();
	Log::log.file = std::fstream(filename, std::fstream::out);
}
