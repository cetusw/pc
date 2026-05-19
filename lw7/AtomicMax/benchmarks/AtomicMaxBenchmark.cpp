#include "../AtomicMax.h"
#include "../AtomicMaxWithLock.h"

#include <benchmark/benchmark.h>

#include <latch>
#include <thread>
#include <vector>

constexpr int VALUES_PER_THREAD = 100000;

template <typename AtomicMaxType>
void AtomicMaxBenchmark(benchmark::State& state)
{
	const int numThreads = static_cast<int>(state.range(0));

	for (auto _ : state)
	{
		state.PauseTiming();

		AtomicMaxType maxValue(0);
		std::latch startLatch(numThreads);
		std::vector<std::jthread> threads;

		state.ResumeTiming();

		for (int threadIndex = 0; threadIndex < numThreads; ++threadIndex)
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

		benchmark::DoNotOptimize(maxValue.GetValue());
		benchmark::DoNotOptimize(startLatch);
	}
}

BENCHMARK_TEMPLATE(AtomicMaxBenchmark, AtomicMax<int>)
	->DenseRange(1, 30, 1)
	->Unit(benchmark::kMillisecond);

BENCHMARK_TEMPLATE(AtomicMaxBenchmark, AtomicMaxWithLock<int>)
	->DenseRange(1, 30, 1)
	->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();