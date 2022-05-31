#include "logger.h"

std::ofstream CLogger::m_file;

void CLogger::Initialize(const char* file)
{
	m_file = std::ofstream(file, std::ofstream::out);
}

void CLogger::Shutdown()
{
	m_file.close();
}
