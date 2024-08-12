#pragma once

#include <filesystem>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <cstdio>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
template<typename TP>
std::time_t to_time_t(TP tp) {
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
	return system_clock::to_time_t(sctp);
}

namespace da {

	static const char endl[] = "\n";

	class CStream {
	public:
		std::ofstream stream;
		inline CStream& operator<<(const char* str) {
			std::cout << str;
			stream << str;
			return *this;
		}
		inline CStream& operator<<(const std::string& str) {
			std::cout << str;
			stream << str;
			return *this;
		}
		inline CStream& operator<<(const std::filesystem::path& str) {
			std::cout << str;
			stream << str;
			return *this;
		}
	};

	static CStream cout;
}