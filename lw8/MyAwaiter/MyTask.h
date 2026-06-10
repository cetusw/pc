#pragma once

#include <coroutine>
#include <exception>
#include <utility>

class MyTask
{
public:
	struct promise_type;
	using CoroHandle = std::coroutine_handle<promise_type>;

	// NOLINTBEGIN(readability-convert-member-functions-to-static)
	struct promise_type
	{
		MyTask get_return_object()
		{
			return MyTask{ CoroHandle::from_promise(*this) };
		}

		std::suspend_never initial_suspend()
		{
			return {};
		}

		std::suspend_always final_suspend() noexcept
		{
			return {};
		}

		void return_void() noexcept
		{
		}

		void unhandled_exception()
		{
			std::terminate();
		}
	};
	// NOLINTEND(readability-convert-member-functions-to-static)

	explicit MyTask(const CoroHandle h)
		: m_handle(h)
	{
	}

	MyTask& operator=(const MyTask&) = delete;
	MyTask(MyTask&& other) noexcept
		: m_handle(std::exchange(other.m_handle, nullptr))
	{
	}

	MyTask& operator=(MyTask&& other) noexcept
	{
		if (this != &other)
		{
			if (m_handle)
			{
				m_handle.destroy();
			}

			m_handle = std::exchange(other.m_handle, nullptr);
		}

		return *this;
	}

	~MyTask()
	{
		if (m_handle)
		{
			m_handle.destroy();
		}
	}

	void Resume() const
	{
		if (m_handle && !m_handle.done())
		{
			m_handle.resume();
		}
	}

private:
	CoroHandle m_handle;
};
