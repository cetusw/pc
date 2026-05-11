#pragma once
#include "StopToken.h"

class StopSource
{
public:
	StopSource();

	void RequestStop() const;
	[[nodiscard]] StopToken GetToken() const;

private:
	std::shared_ptr<std::atomic_bool> m_state;
};
