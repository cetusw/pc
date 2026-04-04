#include "Worker.h"

Worker::Worker(BoundedBlockingQueue<int>& queue)
	: m_queue(queue)
	, m_totalProcessed(0)
{
}
int Worker::GetTotalProcessed() const
{
	return m_totalProcessed;
}