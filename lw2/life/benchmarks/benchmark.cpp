#include <benchmark/benchmark.h>
#include "../Model/Board.h"

static void LifeBenchmark(benchmark::State& state) {
	constexpr int width = 2000;
	constexpr int height = 2000;
	const int numThreads = static_cast<int>(state.range(0));

	Board board(width, height);
	board.GenerateCells(0.5f);

	board.StartSynchronizer(numThreads);

	for (auto _ : state) {
		board.Step();
	}

	state.SetItemsProcessed(state.iterations() * width * height);
	state.counters["Threads"] = numThreads;
}

BENCHMARK(LifeBenchmark)
    ->DenseRange(1, 12, 1)
	->Unit(benchmark::kMillisecond)
	->UseRealTime();

BENCHMARK_MAIN();