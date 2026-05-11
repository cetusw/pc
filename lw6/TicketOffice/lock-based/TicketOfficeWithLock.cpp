#include "TicketOfficeWithLock.h"
#include <stdexcept>

TicketOfficeWithLock::TicketOfficeWithLock(const int numTickets)
	: m_numTickets(numTickets)
{
	if (numTickets < 0)
	{
		throw std::invalid_argument("Tickets number must be non-negative");
	}
}

int TicketOfficeWithLock::SellTickets(const int ticketsToBuy)
{
	if (ticketsToBuy <= 0)
	{
		throw std::invalid_argument("Tickets to buy number must be positive");
	}

	std::lock_guard lock(m_mutex);
	const int ticketsToSell = std::min(ticketsToBuy, m_numTickets);
	m_numTickets -= ticketsToSell;

	return ticketsToSell;
}

int TicketOfficeWithLock::GetTicketsLeft() const noexcept
{
	std::lock_guard lock(m_mutex);
	return m_numTickets;
}
