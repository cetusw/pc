#include "Producer.h"
#include "../utils/Rand.h"
#include <chrono>
#include <thread>

void Producer::Run(const std::stop_token stopToken)
{
	while (!stopToken.stop_requested())
	{
		const int value = Rand::Get(1, 1000);
		try
		{
			m_queue.Push(value);
			m_totalProcessed++;
		}
		catch (const std::logic_error&)
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}