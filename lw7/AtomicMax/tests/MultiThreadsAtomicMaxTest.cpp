#include "../AtomicMax.h"

#include <gtest/gtest.h>
#include <latch>
#include <thread>

constexpr int NUM_THREADS = 16;
constexpr int VALUES_PER_THREAD = 10000;

TEST(AtomicMax, MultipleThreadsAlwaysHigher)
{
	AtomicMax maxValue(0);
	std::latch startLatch(NUM_THREADS);

	std::vector<std::thread> threads;

	for (int threadIndex = 0; threadIndex < NUM_THREADS; ++threadIndex)
	{
		threads.emplace_back([&, threadIndex] {
			startLatch.arrive_and_wait();

			for (int i = 0; i < VALUES_PER_THREAD; ++i)
			{
				maxValue.Update(threadIndex * VALUES_PER_THREAD + i);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(maxValue.GetValue(), NUM_THREADS * VALUES_PER_THREAD - 1);
}

TEST(AtomicMax, MultipleThreadsAlwaysLower)
{
	AtomicMax maxValue(NUM_THREADS * VALUES_PER_THREAD);
	std::latch startLatch(NUM_THREADS);

	std::vector<std::thread> threads;

	for (int threadIndex = 0; threadIndex < NUM_THREADS; ++threadIndex)
	{
		threads.emplace_back([&, threadIndex] {
			startLatch.arrive_and_wait();

			for (int i = 0; i < VALUES_PER_THREAD; ++i)
			{
				maxValue.Update(threadIndex * VALUES_PER_THREAD + i);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(maxValue.GetValue(), NUM_THREADS * VALUES_PER_THREAD);
}

TEST(AtomicMax, MultipleThreadsMixedValues)
{
	AtomicMax maxValue(NUM_THREADS * VALUES_PER_THREAD / 2);
	std::latch startLatch(NUM_THREADS);

	std::vector<std::thread> threads;

	for (int threadIndex = 0; threadIndex < NUM_THREADS; ++threadIndex)
	{
		threads.emplace_back([&, threadIndex] {
			startLatch.arrive_and_wait();

			for (int i = 0; i < VALUES_PER_THREAD; ++i)
			{
				maxValue.Update(threadIndex * VALUES_PER_THREAD + i);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(maxValue.GetValue(), NUM_THREADS * VALUES_PER_THREAD - 1);
}
