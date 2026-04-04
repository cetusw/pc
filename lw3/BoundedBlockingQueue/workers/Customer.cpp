#include "Customer.h"
#include <chrono>
#include <thread>

void Client::Run(const std::stop_token stopToken)
{
	int value;
	while (!stopToken.stop_requested())
	{
		if (m_queue.PopFor(value, std::chrono::milliseconds(500)))
		{
			m_totalProcessed++;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			if (m_queue.IsClosed() && m_queue.GetSize() == 0)
			{
				break;
			}
		}
	}
}

std::string Client::GetName() const
{
	return "Consumer";
}