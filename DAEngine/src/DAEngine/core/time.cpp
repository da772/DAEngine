#include "time.h"

namespace da::core
{
	double CTime::ms_timeStep = 0.0;
	uint64_t CTime::ms_lastFrameTime = 0;
	uint64_t CTime::ms_frameCount = 0;
	uint64_t CTime::ms_fps = 0;
	uint64_t CTime::ms_totalFrameCount = 0;
	double CTime::ms_timer = 0.0;
	std::vector<da::core::FTimerDelegate> CTime::ms_timers;


	uint64_t CTime::getEpochTimeNS()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	
	}

	uint64_t CTime::getEpochTimeMS()
	{
		return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	uint64_t CTime::getEpochTimeSec()
	{
		return  std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	double CTime::getTimeStep()
	{
		return ms_timeStep;
	}

	uint64_t CTime::getFps()
	{
		return ms_fps;
	}

	double CTime::newFrame()
	{
		double time = CTime::getEpochTimeMS();
		ms_timeStep = (time - ms_lastFrameTime) / 1000.0;
		ms_lastFrameTime = time;

		ms_totalFrameCount++;
		ms_frameCount++;
		ms_timer += ms_timeStep;

		if (ms_timer >= 1.0) {
			ms_fps = ms_frameCount;
			ms_frameCount = 0;
			ms_timer = 0.0;
		}
		
		for (int i = ms_timers.size()-1; i >= 0; --i) {
			FTimerDelegate& timer = ms_timers[i];
			if ((size_t)time - timer.StartTime >= timer.TimeMS) {
				timer.Callback();
				ms_timers.erase(ms_timers.begin() + i);
			}
		}

		return ms_timeStep;
	}

	uint64_t CTime::getFrameCount()
	{
		return ms_totalFrameCount;
	}

	void CTime::addTimerCallback(size_t ms, const std::function<void()>& callback)
	{
		FTimerDelegate timer = { ms, getEpochTimeMS(), callback };
		ms_timers.push_back(timer);
	}

}