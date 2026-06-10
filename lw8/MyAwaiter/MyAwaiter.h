#pragma once
#include <coroutine>

class MyAwaiter
{
public:
	// NOLINTBEGIN(readability-convert-member-functions-to-static)
	[[nodiscard]] bool await_ready() const noexcept
	{
		return false;
	}

	void await_suspend(std::coroutine_handle<>) const noexcept
	{
	}

	[[nodiscard]] int await_resume() const noexcept
	{
		return m_x + m_y;
	}
	// NOLINTEND(readability-convert-member-functions-to-static)

	MyAwaiter(const int x, const int y)
		: m_x(x)
		, m_y(y)
	{
	}

private:
	int m_x;
	int m_y;
};
