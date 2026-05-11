#include "../lock-free/TicketOffice.h"
#include "utils/Rand.h"
#include <gtest/gtest.h>
#include <latch>
#include <thread>

constexpr int INITIAL_TICKETS = 1000;
constexpr int THREAD_COUNT = 16;
constexpr int ATTEMPTS = 1000;

TEST(MultipleThreadsTicketOfficeTest, SellsOneTicketPerAttempt)
{
	TicketOffice office(INITIAL_TICKETS);
	std::atomic totalSold = 0;
	std::latch startLatch(THREAD_COUNT);
	std::vector<std::jthread> threads;

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back([&office, &totalSold, &startLatch] {
			startLatch.arrive_and_wait();

			for (int j = 0; j < ATTEMPTS; ++j)
			{
				totalSold.fetch_add(
					office.SellTickets(1),
					std::memory_order_relaxed);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(totalSold.load(std::memory_order_relaxed), INITIAL_TICKETS);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}

TEST(MultipleThreadsTicketOfficeTest, SellsRandomNumberOfTicketsPerAttempt)
{
	TicketOffice office(INITIAL_TICKETS);
	std::atomic totalSold = 0;
	std::latch startLatch(THREAD_COUNT);
	std::vector<std::jthread> threads;

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back([&office, &totalSold, &startLatch] {
			startLatch.arrive_and_wait();

			for (int j = 0; j < ATTEMPTS; ++j)
			{
				const int ticketsToBuy = Rand::Get(1, 10);
				totalSold.fetch_add(
					office.SellTickets(ticketsToBuy),
					std::memory_order_relaxed);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(totalSold.load(std::memory_order_relaxed), INITIAL_TICKETS);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}

TEST(MultipleThreadsTicketOfficeTest, SellsAllTickets)
{
	TicketOffice office(INITIAL_TICKETS);
	std::atomic totalSold = 0;
	std::latch startLatch(THREAD_COUNT);
	std::vector<std::jthread> threads;

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back([&office, &totalSold, &startLatch] {
			startLatch.arrive_and_wait();

			totalSold.fetch_add(
				office.SellTickets(INITIAL_TICKETS),
				std::memory_order_relaxed);
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(totalSold.load(std::memory_order_relaxed), INITIAL_TICKETS);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}

TEST(MultipleThreadsTicketOfficeTest, SellsMoreThenAvailable)
{
	TicketOffice office(INITIAL_TICKETS);
	std::atomic totalSold = 0;
	std::latch startLatch(THREAD_COUNT);
	std::vector<std::jthread> threads;

	for (int i = 0; i < THREAD_COUNT; ++i)
	{
		threads.emplace_back([&office, &totalSold, &startLatch] {
			startLatch.arrive_and_wait();

			totalSold.fetch_add(
				office.SellTickets(INITIAL_TICKETS + 1),
				std::memory_order_relaxed);
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	EXPECT_EQ(totalSold.load(std::memory_order_relaxed), INITIAL_TICKETS);
	EXPECT_EQ(office.GetTicketsLeft(), 0);
}