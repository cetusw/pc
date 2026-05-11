#include "StopToken.h"

bool StopToken::StopRequested() const
{
	return m_state && m_state->load(std::memory_order_relaxed);
}

StopToken::StopToken(std::shared_ptr<std::atomic_bool> state)
	: m_state(std::move(state))
{
}
