#include "logger.h"
#include "test_manager.h"
#include "utility.h"

int main(int argc, char** argv)
{
	Logger::Initalize(utility::GetLogFileName().c_str()) ;

	TestManager manager;

	manager.RunTests();

	Logger::Shutdown();

	return 0;
}