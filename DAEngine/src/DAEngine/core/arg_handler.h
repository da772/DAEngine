#pragma once

namespace da::core {

	class CArgHandler
	{
	public:
		static void initialize(int argc, const char** argv);
		static void shutdown();

	private:
		static int s_argc;
		static const char** s_argv;

	};

}