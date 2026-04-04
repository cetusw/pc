#include "SignalHandler.h"
#include <csignal>

void SignalHandler::Setup()
{
	std::signal(SIGINT, [](int) { (void)m_stopSource.request_stop(); });
	std::signal(SIGTERM, [](int) { (void)m_stopSource.request_stop(); });
}

std::stop_source& SignalHandler::GetSource()
{
	return m_stopSource;
}

std::stop_token SignalHandler::GetToken()
{
	return m_stopSource.get_token();
}