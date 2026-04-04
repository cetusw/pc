#include "Customer.h"
#include <chrono>
#include <thread>

void Client::Run(const std::stop_token stopToken)
{
	int value;
	while (!stopToken.stop_requested())
	{
		if (!m_queue.Pop(value)) {
			break;
		}

		m_totalProcessed++;

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}