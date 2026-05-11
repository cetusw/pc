#include "../lock-free/TicketOffice.h"
#include <gtest/gtest.h>

TEST(SingleThreadTicketOfficeTest, SellsRequestedTickets)
{
	TicketOffice office(10);

	EXPECT_EQ(office.SellTickets(3), 3);
	EXPECT_EQ(office.GetTicketsLeft(), 7);
}

TEST(SingleThreadTicketOfficeTest, DoesNotSellMoreThanAvailable)
{
	TicketOffice office(5);

	EXPECT_EQ(office.SellTickets(10), 5);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}

TEST(SingleThreadTicketOfficeTest, ReturnsZeroWhenNoTicketsLeft)
{
	TicketOffice office(0);

	EXPECT_EQ(office.SellTickets(1), 0);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}

TEST(SingleThreadTicketOfficeTest, ThrowsIfTicketsToBuyIsNotPositive)
{
	TicketOffice office(10);

	EXPECT_THROW(office.SellTickets(0), std::invalid_argument);
	EXPECT_THROW(office.SellTickets(-1), std::invalid_argument);
}

TEST(SingleThreadTicketOfficeTest, ThrowsIfNumberOfTicketsIsNegative)
{
	EXPECT_THROW(TicketOffice(-1), std::invalid_argument);
}