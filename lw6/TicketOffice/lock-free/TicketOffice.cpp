#include "TicketOffice.h"
#include <stdexcept>

TicketOffice::TicketOffice(const int numTickets)
	: m_numTickets(numTickets)
{
	if (numTickets < 0)
	{
		throw std::invalid_argument("Tickets number must be non-negative");
	}
}

int TicketOffice::SellTickets(const int ticketsToBuy)
{
	if (ticketsToBuy <= 0)
	{
		throw std::invalid_argument("Tickets to buy number must be positive");
	}

	int ticketsLeft = m_numTickets.load(std::memory_order_relaxed);

	while (ticketsLeft > 0)
	{
		const int ticketsToSell = std::min(ticketsToBuy, ticketsLeft);
		const int newTicketsLeft = ticketsLeft - ticketsToSell;

		if (m_numTickets.compare_exchange_weak(ticketsLeft, newTicketsLeft))
		{
			return ticketsToSell;
		}
	}

	return 0;
}

int TicketOffice::GetTicketsLeft() const noexcept
{
	return m_numTickets.load(std::memory_order_relaxed);
}
