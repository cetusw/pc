#ifndef ACTOR_H
#define ACTOR_H

#include "Bank.h"

#include <memory>

constexpr int DEFAULT_MIN = 0;

class Actor;
class ActorWithBankAccount;

class Actor
{
public:
	explicit Actor(Money cash);
	virtual ~Actor() = default;

	[[nodiscard]] Money GetCash() const;

	[[nodiscard]] bool SendCash(Actor& actor, Money amount);

	virtual void Act(Actors& actors) = 0;

protected:
	[[nodiscard]] static int GenerateRandomInt(int maxValue);

	void AddCash(Money amount);

	bool SpendCash(Money amount);

private:
	Money m_cash;
};

#endif // ACTOR_H
