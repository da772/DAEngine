#pragma once

#include <thread>
#include <vector>

class CThreadPool
{
public:
	inline void wait()
	{
		while (!ms_threads.empty())
		{
			for (int i = ms_threads.size() - 1; i >= 0; --i)
			{
				if (ms_threads[i].joinable())
				{
					ms_threads[i].join();
					ms_threads.erase(ms_threads.begin() + i);
				}
			}
		}
	}

	std::vector<std::thread> ms_threads;

	static CThreadPool& get() { return ms_threadPool; }

private:
	static CThreadPool ms_threadPool;
};
