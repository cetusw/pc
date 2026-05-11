#include "../lock-based/TicketOfficeWithLock.h"
#include "../lock-free/TicketOffice.h"
#include <atomic>
#include <benchmark/benchmark.h>
#include <latch>
#include <thread>
#include <vector>

constexpr int INITIAL_TICKETS = 1000000;
constexpr int ATTEMPTS = 1000000;

template <typename TicketOfficeType>
static void TicketOfficeBenchmark(benchmark::State& state)
{
	const int threadCount = static_cast<int>(state.range(0));

	for (auto _ : state)
	{
		TicketOfficeType office(INITIAL_TICKETS);
		std::atomic totalSold = 0;

		std::latch startLatch(threadCount);
		std::vector<std::jthread> threads;
		threads.reserve(threadCount);

		for (int i = 0; i < threadCount; ++i)
		{
			threads.emplace_back([&office, &totalSold, &startLatch] {
				startLatch.arrive_and_wait();

				int localSold = 0;

				for (int j = 0; j < ATTEMPTS; ++j)
				{
					localSold += office.SellTickets(1);
				}

				totalSold.fetch_add(localSold, std::memory_order_relaxed);
			});
		}

		for (auto& thread : threads)
		{
			thread.join();
		}

		benchmark::DoNotOptimize(totalSold.load(std::memory_order_relaxed));
		benchmark::DoNotOptimize(office.GetTicketsLeft());
	}
}

BENCHMARK_TEMPLATE(TicketOfficeBenchmark, TicketOffice)
	->DenseRange(1, 30, 1)
	->Unit(benchmark::kMillisecond);

BENCHMARK_TEMPLATE(TicketOfficeBenchmark, TicketOfficeWithLock)
	->DenseRange(1, 30, 1)
	->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();