#pragma once
#include "BoundedBlockingQueue.h"
#include "workers/Worker.h"
#include <memory>
#include <thread>
#include <vector>

class Application
{
public:
	Application(int numProducers, int numConsumers, int capacity);
	void Run();

private:
	std::vector<std::unique_ptr<Worker>> m_workers;
	std::vector<std::thread> m_threads;

	BoundedBlockingQueue<int> m_queue;
	int m_numProducers;
	int m_numConsumers;

	void CreateWorkers();
	void StartThreads();
	void JoinThreads();
	void PrintStatistics() const;
};