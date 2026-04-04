#pragma once
#include "Worker.h"

class Producer final : public Worker
{
public:
	explicit Producer(BoundedBlockingQueue<int>& queue)
		: Worker(queue)
	{
	}
	void Run(std::stop_token stopToken) override;
	std::string GetName() const override;
};