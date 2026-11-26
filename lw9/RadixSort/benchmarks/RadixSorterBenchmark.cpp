#include "OpenClContext.h"
#include "RadixSorter.h"
#include "utils/Utils.h"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <execution>
#include <random>
#include <vector>

void BenchmarkGpuRadixSort(benchmark::State& state)
{
	const auto size = static_cast<std::size_t>(state.range(0));
	const auto source = Utils::GenerateRandomData(size);

	OpenClContext openCl;
	auto sorter = Utils::CreateSorter(openCl);

	for (auto _ : state)
	{
		auto data = source;

		benchmark::DoNotOptimize(data.data());

		auto result = sorter.Sort(data);

		benchmark::DoNotOptimize(result.data());
		benchmark::ClobberMemory();
	}
}

void BenchmarkCpuParallelSort(benchmark::State& state)
{
	const auto size = static_cast<std::size_t>(state.range(0));
	const auto source = Utils::GenerateRandomData(size);

	for (auto _ : state)
	{
		auto data = source;

		std::sort(std::execution::par, data.begin(), data.end());

		benchmark::DoNotOptimize(data);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BenchmarkGpuRadixSort)
	->Arg(1'000)
	->Arg(10'000)
	->Arg(100'000)
	->Arg(1'000'000)
	->Unit(benchmark::kMillisecond)
	->UseRealTime();

BENCHMARK(BenchmarkCpuParallelSort)
	->Arg(1'000)
	->Arg(10'000)
	->Arg(100'000)
	->Arg(1'000'000)
	->Unit(benchmark::kMillisecond)
	->UseRealTime();