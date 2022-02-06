#include "logger.h"

std::ofstream Logger::m_file;

void Logger::Initalize(const char* file)
{
	m_file = std::ofstream(file, std::ofstream::out);
}

void Logger::Shutdown()
{
	m_file.close();
}
