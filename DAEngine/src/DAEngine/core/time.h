#pragma once

namespace da::core
{
	struct FTimerDelegate {
		size_t TimeMS;
		size_t StartTime;
		std::function<void()> Callback;
	};

	class CTime
	{
	public:
		static uint64_t getEpochTimeNS();
		static uint64_t getEpochTimeMS();
		static uint64_t getEpochTimeSec();
		static double newFrame();
		static double getTimeStep();
		static uint64_t getFps();
		static uint64_t getFrameCount();
		static void addTimerCallback(size_t ms, const std::function<void()>& callback);


	private:
		static uint64_t ms_frameCount;
		static uint64_t ms_fps;
		static uint64_t ms_lastFrameTime;
		static double ms_timeStep;
		static double ms_timer;
		static uint64_t ms_totalFrameCount;
		static std::vector<FTimerDelegate> ms_timers;
	};
}