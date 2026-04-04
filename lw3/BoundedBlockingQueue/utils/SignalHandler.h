#pragma once
#include <stop_token>

class SignalHandler
{
public:
	static void Setup();
	static std::stop_source& GetSource();
	static std::stop_token GetToken();

private:
	static inline std::stop_source m_stopSource;
};
