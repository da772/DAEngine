
#include "worker_pool.h"

namespace da::core
{
	std::vector < std::thread > CWorkerPool::ms_threads;
	std::queue <std::function<void()>> CWorkerPool::ms_jobQueue;
	std::mutex CWorkerPool::ms_queueMutex;
	std::mutex CWorkerPool::ms_threadMutex;
	std::condition_variable CWorkerPool::ms_condition;
	bool CWorkerPool::ms_terminateThreads = false;
	std::mutex CWorkerPool::ms_PauseMutex;
	std::vector<std::function<void()>> CWorkerPool::m_mainFuncs;
	std::mutex CWorkerPool::ms_mainThreadMutex;


	void CWorkerPool::addJob(std::function<void()> func)
	{
		{
			std::lock_guard<std::mutex> g(CWorkerPool::ms_queueMutex);
			CWorkerPool::ms_jobQueue.push(func);
		}
		CWorkerPool::ms_condition.notify_one();

	}

	void CWorkerPool::initialize()
	{
		int numThreads = std::thread::hardware_concurrency();
		if (numThreads == 0) numThreads = 1;
		LOG_INFO(ELogChannel::Core, "CWorkerPool: Created %d threads", numThreads);
		std::lock_guard<std::mutex> g(CWorkerPool::ms_threadMutex);
		for (int i = 0; i < numThreads; i++) {
			ms_threads.push_back(std::thread([]() {
				PROFILE_THREAD("Worker")
				std::function<void()> job;

				while (true) {
					{
						{
							ms_PauseMutex.lock();
							ms_PauseMutex.unlock();
							std::unique_lock<std::mutex> lock(CWorkerPool::ms_queueMutex);
							ms_condition.wait(lock, [] {return !CWorkerPool::ms_jobQueue.empty() || CWorkerPool::ms_terminateThreads; });
							if (CWorkerPool::ms_jobQueue.empty()) break;
							{
								std::lock_guard<std::mutex> g(CWorkerPool::ms_threadMutex);
								if (CWorkerPool::ms_terminateThreads) {
									return;
								}

							}
							job = CWorkerPool::ms_jobQueue.front();
							CWorkerPool::ms_jobQueue.pop();
						}
						PROFILE_NAME("job")
						job();
					}
				}
				}));
		}
	}

	void CWorkerPool::shutdown()
	{
		{
			std::lock_guard<std::mutex> g(CWorkerPool::ms_threadMutex);
			CWorkerPool::ms_terminateThreads = true;
		}
		CWorkerPool::ms_condition.notify_all();
		for (std::thread& t : CWorkerPool::ms_threads) {
			t.join();
		}

		CWorkerPool::ms_threads.clear();

	}

	std::thread::id CWorkerPool::getThreadId()
	{
		return std::this_thread::get_id();
	}

	void CWorkerPool::unpauseThreads()
	{
		ms_PauseMutex.unlock();
	}

	void CWorkerPool::pauseThreads()
	{
		ms_PauseMutex.lock();
	}

	std::mutex& CWorkerPool::getPauseMutex()
	{
		return ms_PauseMutex;
	}

	void CWorkerPool::join()
	{
		for (std::thread& t : CWorkerPool::ms_threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}

	void CWorkerPool::addMainJob(std::function<void()> func)
	{
		std::lock_guard<std::mutex> guard(ms_mainThreadMutex);
		m_mainFuncs.push_back(func);
	}

	void CWorkerPool::update()
	{
		std::lock_guard<std::mutex> guard(ms_mainThreadMutex);
		for (const std::function<void()>& f : m_mainFuncs) {
			f();
		}
		m_mainFuncs = {};
	}


}