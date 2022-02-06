#pragma once
#include <fstream>

class Logger
{
public:
	static void Initalize(const char* output);
	static void Shutdown();

	template<typename ... Args>
	static inline void Log(const char* format, Args&& ... args)
	{
		char buff[4096] = { 0 };
		sprintf_s(buff, sizeof(buff), format, std::forward<Args>(args)...);

		printf("%s\n", buff);

		Logger::m_file << buff << "\n";
		m_file.flush();
	}
private:
	static std::ofstream m_file;

};
