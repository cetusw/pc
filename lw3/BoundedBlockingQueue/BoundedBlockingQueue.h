#pragma once
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>

template <typename T>
class BoundedBlockingQueue
{
public:
	explicit BoundedBlockingQueue(const size_t capacity)
		: m_capacity(capacity)
		, m_isClosed(false)
		, m_lockCount(0)
		, m_maxQueueSize(0)
	{
		if (capacity <= 0)
		{
			throw std::invalid_argument("Capacity must be positive");
		}
	}

	BoundedBlockingQueue(const BoundedBlockingQueue&) = delete;
	BoundedBlockingQueue& operator=(const BoundedBlockingQueue&) = delete;

	void Push(T value)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();

		m_notFull.wait(lock, [this] {
			return m_queue.size() < m_capacity || m_isClosed;
		});

		if (m_isClosed)
		{
			throw std::logic_error("Queue is closed");
		}
		m_queue.push_back(std::move(value));
		UpdateMaxSize();
		lock.unlock();

		m_notEmpty.notify_one();
	}

	bool TryPush(T value)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();
		if (m_queue.size() >= m_capacity || m_isClosed)
		{
			return false;
		}
		m_queue.push_front(std::move(value));
		UpdateMaxSize();
		lock.unlock();

		m_notEmpty.notify_one();
		return true;
	}

	bool Pop(T& out)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();
		m_notEmpty.wait(lock, [this] {
			return !m_queue.empty() || m_isClosed;
		});
		if (m_isClosed && m_queue.empty())
		{
			return false;
		}
		out = std::move(m_queue.front());
		m_queue.pop_front();
		lock.unlock();

		m_notFull.notify_one();
		return true;
	}

	bool TryPop(T& out)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();
		if (m_queue.empty())
		{
			return false;
		}
		out = std::move(m_queue.front());
		m_queue.pop();
		lock.unlock();

		m_notFull.notify_one();
		return true;
	}

	bool PushFor(T value, std::chrono::milliseconds timeout)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();

		const bool success = m_notFull.wait_for(lock, timeout, [this] {
			return m_queue.size() < m_capacity || m_isClosed;
		});

		if (!success || m_isClosed)
		{
			return false;
		}

		m_queue.push_back(std::move(value));
		UpdateMaxSize();
		lock.unlock();
		m_notEmpty.notify_one();
		return true;
	}

	bool PopFor(T& out, std::chrono::milliseconds timeout)
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();

		const bool success = m_notEmpty.wait_for(lock, timeout, [this] {
			return !m_queue.empty() || m_isClosed;
		});

		if (!success || m_queue.empty())
		{
			return false;
		}

		out = std::move(m_queue.front());
		m_queue.pop_front();
		lock.unlock();
		m_notFull.notify_one();
		return true;
	}

	void Close()
	{
		std::unique_lock lock(m_mutex);
		IncrementLockCount();
		m_isClosed = true;
		m_notFull.notify_all();
		m_notEmpty.notify_all();
	}

	[[nodiscard]] size_t GetSize() const
	{
		std::unique_lock lock(m_mutex);
		return m_queue.size();
	}

	[[nodiscard]] size_t GetCapacity() const
	{
		return m_capacity;
	}

	[[nodiscard]] bool IsClosed() const
	{
		std::unique_lock lock(m_mutex);
		return m_isClosed;
	}

	[[nodiscard]] long long GetLockCount() const
	{
		return m_lockCount.load(std::memory_order_relaxed);
	}

	[[nodiscard]] size_t GetMaxQueueSize() const
	{
		return m_maxQueueSize.load(std::memory_order_relaxed);
	}

private:
	mutable std::mutex m_mutex;
	std::deque<T> m_queue;
	size_t m_capacity;
	std::condition_variable m_notFull;
	std::condition_variable m_notEmpty;
	bool m_isClosed;
	std::atomic<long long> m_lockCount;
	std::atomic<size_t> m_maxQueueSize;

	void IncrementLockCount()
	{
		m_lockCount.fetch_add(1, std::memory_order_relaxed);
	}

	void UpdateMaxSize()
	{
		const size_t currentSize = m_queue.size();
		size_t oldMax = m_maxQueueSize.load(std::memory_order_relaxed);
		while (currentSize > oldMax)
		{
			if (m_maxQueueSize.compare_exchange_weak(oldMax, currentSize, std::memory_order_relaxed))
			{
				break;
			}
		}
	}
};
