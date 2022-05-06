#pragma once
#include <fstream>
#include <regex>

class CLogger
{
public:
	static void Initialize(const char* output);
	static void Shutdown();

	template<typename ... Args>
	static inline void Log(const char* format, Args&& ... args)
	{
		char buff[4096] = { 0 };
		sprintf_s(buff, sizeof(buff), format, std::forward<Args>(args)...);

		printf("%s\n", buff);

		std::regex match_str = std::regex("\033[[][0-9][0-9][m]?");

		std::stringstream result;
		size_t sz = strlen(buff);
		std::regex_replace(std::ostream_iterator<char>(result), buff, &buff[sz-1], match_str, "");

		CLogger::m_file << result.str() << "\n";
		m_file.flush();
	}

	template<typename ... Args>
	static inline std::string Format(const char* format, Args&& ... args)
	{
		char buff[4096] = { 0 };
		sprintf_s(buff, sizeof(buff), format, std::forward<Args>(args)...);

		return std::string(buff);
	}
private:
	static std::ofstream m_file;

};

namespace color
{
	static const char FG_RED[] = "\033[31m";
	static const char FG_GREEN[] = "\033[32m";
	static const char FG_YELLOW[] = "\033[33m";
	static const char FG_BLUE[] = "\033[34m";
	static const char FG_CYAN[] = "\033[36m";
	static const char FG_DEFAULT[] = "\033[39m";
}
