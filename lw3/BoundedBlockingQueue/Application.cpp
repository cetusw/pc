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

	for (const auto& w : m_workers)
	{
		if (dynamic_cast<const Producer*>(w.get()))
		{
			totalProduced += w->GetTotalProcessed();
		}
		else if (dynamic_cast<const Client*>(w.get()))
		{
			totalConsumed += w->GetTotalProcessed();
		}
	}

	std::cout << "-----------------------------" << std::endl;
	std::cout << "Produced: " << totalProduced << std::endl;
	std::cout << "Consumed: " << totalConsumed << std::endl;
	std::cout << "Left in Queue: " << m_queue.GetSize() << std::endl;
}