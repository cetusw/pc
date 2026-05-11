#pragma once
#include <mutex>

class TicketOfficeWithLock
{
public:
	explicit TicketOfficeWithLock(int numTickets);

	TicketOfficeWithLock(const TicketOfficeWithLock&) = delete;
	TicketOfficeWithLock& operator=(const TicketOfficeWithLock&) = delete;

	int SellTickets(int ticketsToBuy);

	[[nodiscard]] int GetTicketsLeft() const noexcept;

private:
	mutable std::mutex m_mutex;
	int m_numTickets;
};
