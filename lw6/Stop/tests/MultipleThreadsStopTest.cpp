#include "../StopSource.h"
#include "../StopToken.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

TEST(StopSource, TokenCanStopSingleThread)
{
	const StopSource source;
	StopToken token = source.GetToken();

	std::atomic_bool workerStopped = false;

	std::jthread worker([token, &workerStopped] {
		while (!token.StopRequested())
		{
			std::this_thread::yield();
		}

		workerStopped.store(true, std::memory_order_relaxed);
	});

	source.RequestStop();

	worker.join();

	EXPECT_TRUE(workerStopped.load());
}

TEST(StopSource, TokenCanStopMultipleThreads)
{
	const StopSource source;
	StopToken token = source.GetToken();
	constexpr int threadCount = 10;
	std::atomic workerStoppedCount = 0;
	std::vector<std::jthread> workers;

	for (int i = 0; i < threadCount; ++i)
	{
		workers.emplace_back([token, &workerStoppedCount] {
			while (!token.StopRequested())
			{
				std::this_thread::yield();
			}

			workerStoppedCount.fetch_add(1, std::memory_order_relaxed);
		});
	}

	source.RequestStop();

	for (auto& worker : workers)
	{
		worker.join();
	}

	EXPECT_TRUE(workerStoppedCount.load(std::memory_order_relaxed) == threadCount);
}