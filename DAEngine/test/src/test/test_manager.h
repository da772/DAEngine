#pragma once

#include <vector>

class ITest;

class CTestManager
{
public:

	CTestManager();
	~CTestManager();

	template<class T>
	inline void RegisterTest()
	{
		m_tests.push_back(new T());
	}

	void RunTests();

private:
	std::vector<ITest*> m_tests;

};
