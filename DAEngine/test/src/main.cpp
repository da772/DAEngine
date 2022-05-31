#include "test/test_manager.h"
#include "util/logger.h"
#include "util/utility.h"

int main(int argc, char** argv)
{
	CLogger::Initialize(utility::GetLogFileName().c_str());

	CTestManager manager;

	manager.RunTests();

	CLogger::Shutdown();

	return 0;
}