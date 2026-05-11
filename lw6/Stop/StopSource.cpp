#include "StopSource.h"
#include <atomic>
#include <memory>

StopSource::StopSource()
	: m_state(std::make_shared<std::atomic_bool>(false))
{
}

void StopSource::RequestStop() const
{
	m_state->store(true, std::memory_order_relaxed);
}

StopToken StopSource::GetToken() const
{
	return StopToken{ m_state };
}