#include "Application.h"
#include "utils/SignalHandler.h"
#include "workers/Customer.h"
#include "workers/Producer.h"
#include <chrono>
#include <iostream>

Application::Application(const int numProducers, const int numConsumers, const int capacity)
	: m_queue(capacity)
	, m_numProducers(numProducers)
	, m_numConsumers(numConsumers)
{
}

void Application::Run()
{
	std::cout << "Starting simulation..." << std::endl;

	SignalHandler::Setup();
	const auto stopToken = SignalHandler::GetToken();

	CreateWorkers();
	StartThreads();

	while (!stopToken.stop_requested())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::cout << "Stop requested. Closing queue..." << std::endl;

	m_queue.Close();

	JoinThreads();
	PrintStatistics();
}

void Application::CreateWorkers()
{
	for (int i = 0; i < m_numProducers; ++i)
	{
		m_workers.push_back(std::make_unique<Producer>(m_queue));
	}
	for (int i = 0; i < m_numConsumers; ++i)
	{
		m_workers.push_back(std::make_unique<Client>(m_queue));
	}
}

void Application::StartThreads()
{
	auto stopToken = SignalHandler::GetToken();
	for (auto& worker : m_workers)
	{
		m_threads.emplace_back(&Worker::Run, worker.get(), stopToken);
	}
}

void Application::JoinThreads()
{
	for (auto& t : m_threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

void Application::PrintStatistics() const
{
	int totalProduced = 0;
	int totalConsumed = 0;
	int producerCount = 0;
	int consumerCount = 0;

	for (const auto& w : m_workers)
	{
		if (w->GetName() == "Producer")
		{
			totalProduced += w->GetTotalProcessed();
			producerCount++;
		}
		else if (w->GetName() == "Consumer")
		{
			totalConsumed += w->GetTotalProcessed();
			consumerCount++;
		}
	}

	std::cout << "-----------------------------" << std::endl;
	std::cout << "STATISTICS:" << std::endl;
	std::cout << "Producers active: " << producerCount << std::endl;
	std::cout << "Consumers active: " << consumerCount << std::endl;
	std::cout << "Total Produced:   " << totalProduced << std::endl;
	std::cout << "Total Consumed:   " << totalConsumed << std::endl;
	std::cout << "Left in Queue:    " << m_queue.GetSize() << std::endl;

	std::cout << "-----------------------------" << std::endl;
	std::cout << "INTERNAL QUEUE STATS:" << std::endl;
	std::cout << "Max Queue Size:   " << m_queue.GetMaxQueueSize() << " / " << m_queue.GetCapacity() << std::endl;
	std::cout << "Total Lock Ops:   " << m_queue.GetLockCount() << std::endl;
}