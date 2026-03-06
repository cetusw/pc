#include "StepSynchronizer.h"

StepSynchronizer::StepSynchronizer(int numThreads, Task task)
	: m_task(std::move(task))
{
	m_syncBarrier = std::make_unique<std::barrier<>>(numThreads + 1);
	try
	{
		for (int i = 0; i < numThreads; ++i)
		{
			m_threads.emplace_back(&StepSynchronizer::WorkerLoop, this, i, numThreads);
		}
	}
	catch (...)
	{
		Shutdown(numThreads - static_cast<int>(m_threads.size()));
		throw;
	}
}

StepSynchronizer::~StepSynchronizer()
{
	Shutdown(0);
}

void StepSynchronizer::ExecuteStep() const
{
	m_syncBarrier->arrive_and_wait();
	m_syncBarrier->arrive_and_wait();
}

void StepSynchronizer::WorkerLoop(const int threadId, const int numThreads) const
{
	while (true)
	{
		m_syncBarrier->arrive_and_wait();
		if (m_isShuttingDown)
		{
			m_syncBarrier->arrive_and_wait();
			break;
		}
		m_task(threadId, numThreads);

		m_syncBarrier->arrive_and_wait();
	}
}

void StepSynchronizer::Shutdown(const int missingThreads)
{
	m_isShuttingDown = true;
	if (m_syncBarrier)
	{
		for (int i = 0; i < missingThreads; ++i)
		{
			m_syncBarrier->arrive_and_drop();
		}
		m_syncBarrier->arrive_and_wait();
		m_syncBarrier->arrive_and_wait();
	}
	for (auto& thread : m_threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	m_threads.clear();
	m_syncBarrier.reset();
}