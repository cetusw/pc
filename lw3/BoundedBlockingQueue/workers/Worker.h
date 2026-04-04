#pragma once
#include "../BoundedBlockingQueue.h"

class Worker
{
public:
	explicit Worker(BoundedBlockingQueue<int>& queue);
	virtual ~Worker() = default;

	virtual void Run(std::stop_token stopToken) = 0;
	[[nodiscard]] int GetTotalProcessed() const;

	virtual std::string GetName() const = 0;

protected:
	BoundedBlockingQueue<int>& m_queue;
	int m_totalProcessed;
};
