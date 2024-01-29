#pragma once

namespace da::core
{
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

	private:
		static uint64_t ms_frameCount;
		static uint64_t ms_fps;
		static uint64_t ms_lastFrameTime;
		static double ms_timeStep;
		static double ms_timer;
		static uint64_t ms_totalFrameCount;
	};
}