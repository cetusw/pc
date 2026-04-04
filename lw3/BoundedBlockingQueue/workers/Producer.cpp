#include "Producer.h"
#include "../utils/Rand.h"
#include <chrono>
#include <thread>

void Producer::Run(const std::stop_token stopToken)
{
	while (!stopToken.stop_requested())
	{
		const int value = Rand::Get(1, 1000);

		if (m_queue.PushFor(value, std::chrono::seconds(1)))
		{
			m_totalProcessed++;
		}
		else
		{
			if (stopToken.stop_requested() || m_queue.IsClosed())
			{
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

std::string Producer::GetName() const
{
	return "Producer";
}