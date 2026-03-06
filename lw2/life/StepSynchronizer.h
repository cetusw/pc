#pragma once
#include <atomic>
#include <barrier>
#include <functional>
#include <memory>

using Task = std::function<void(int threadId, int numThreads)>;

class StepSynchronizer
{
public:
	StepSynchronizer(int numThreads, Task task);
	~StepSynchronizer();

	void ExecuteStep() const;

	StepSynchronizer(const StepSynchronizer&) = delete;
	StepSynchronizer& operator=(const StepSynchronizer&) = delete;

private:
	std::unique_ptr<std::barrier<>> m_syncBarrier;
	std::vector<std::thread> m_threads;
	std::atomic<bool> m_isShuttingDown{ false };
	Task m_task;

	auto WorkerLoop(int threadId, int numThreads) const -> void;
	void Shutdown(int missingThreads);
};
