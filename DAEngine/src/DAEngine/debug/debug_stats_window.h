#pragma once

#ifdef DA_REVIEW

namespace da::debug {

	class CDebugStatsWindow {

	public:
		static void initialize();
		static void update();
		static void shutdown();


		static double s_cpuTime;
		static double s_gpuTime;
		static double s_waitTime;
		static uint64_t s_drawCalls;
		static uint64_t s_gpuMem;
		static uint64_t s_gpuMemMax;
		static uint64_t s_frameCount;
		static std::unordered_map<CHashString, double> s_viewTimes;
	private:
		static bool s_showing;
		static bool s_showViewTimes;

	};
}
#endif