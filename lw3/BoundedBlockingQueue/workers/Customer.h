#pragma once
#include "Worker.h"

class Client final : public Worker
{
public:
	explicit Client(BoundedBlockingQueue<int>& queue)
		: Worker(queue)
	{
	}
	void Run(std::stop_token stopToken) override;
};
