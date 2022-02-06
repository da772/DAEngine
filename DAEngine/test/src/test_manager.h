#pragma once

#include <vector>

class Test;

class TestManager
{
public:

	TestManager();
	~TestManager();

	template<class T>
	inline void RegisterTest()
	{
		m_tests.push_back(new T());
	}

	void RunTests();



private:
	std::vector<Test*> m_tests;

};
