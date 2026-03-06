#include "Actor.h"
#include <random>

Actor::Actor(const Money cash)
	: m_cash(cash)
{
	if (cash < 0)
	{
		throw std::out_of_range("Invalid cash amount");
	}
}

Money Actor::GetCash() const { return m_cash; }

bool Actor::SendCash(Actor& actor, const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Invalid cash amount");
	}
	if (amount > m_cash)
	{
		return false;
	}
	m_cash -= amount;
	actor.AddCash(amount);
	return true;
}

int Actor::GenerateRandomInt(const int maxValue)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(DEFAULT_MIN, maxValue);
	return distrib(gen);
}

void Actor::AddCash(const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Invalid cash amount");
	}
	m_cash += amount;
}

bool Actor::SpendCash(const Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Invalid cash amount");
	}
	if (m_cash < amount)
	{
		return false;
	}
	m_cash -= amount;
	return true;
}
