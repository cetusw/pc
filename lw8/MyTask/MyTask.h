#pragma once

#include <coroutine>
#include <exception>
#include <string>
#include <utility>

class MyTask
{
public:
	struct promise_type;
	using CoroHandle = std::coroutine_handle<promise_type>;

	// NOLINTBEGIN(readability-convert-member-functions-to-static)
	struct promise_type
	{
		std::string result;

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

		void return_value(std::string v) noexcept
		{
			result = std::move(v);
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

	[[nodiscard]] std::string GetResult() const
	{
		return m_handle.promise().result;
	}

private:
	CoroHandle m_handle;
};
