#include "logger.h"
#include "test_manager.h"
#include "utility.h"

int main(int argc, char** argv)
{
	CLogger::Initialize(utility::GetLogFileName().c_str());

	CTestManager manager;

	manager.RunTests();

	CLogger::Shutdown();

	return 0;
}