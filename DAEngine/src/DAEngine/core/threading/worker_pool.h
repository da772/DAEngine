#pragma once

namespace da::core
{
	class CWorkerPool {
	public:
		static void addJob(std::function<void()> func);

		static void initialize();
		static void shutdown();

		static std::thread::id getThreadId();

		static void unpauseThreads();
		static void pauseThreads();


		static std::mutex& getPauseMutex();

		static std::vector < std::thread> ms_threads;
		static std::queue <std::function<void()>> ms_jobQueue;

		static std::mutex ms_queueMutex;
		static std::condition_variable ms_condition;
		static bool ms_terminateThreads;
		static std::mutex ms_threadMutex;
		static std::mutex ms_PauseMutex;


	};
}