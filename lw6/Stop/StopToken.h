#pragma once
#include <atomic>
#include <memory>

class StopToken
{
public:
	StopToken() noexcept = default;

	[[nodiscard]] bool StopRequested() const;

private:
	friend class StopSource;

	explicit StopToken(std::shared_ptr<std::atomic_bool> state);

	std::shared_ptr<std::atomic_bool> m_state;
};
