#ifndef ACTOR_H
#define ACTOR_H

#include "Bank.h"

#include <memory>

constexpr int DEFAULT_MIN = 0;

class Actor;
class ActorWithBankAccount;

struct Actors
{
	std::unique_ptr<ActorWithBankAccount> homer;
	std::unique_ptr<ActorWithBankAccount> marge;
	std::unique_ptr<Actor> bart;
	std::unique_ptr<Actor> liza;
	std::unique_ptr<ActorWithBankAccount> apu;
	std::unique_ptr<ActorWithBankAccount> mrBurns;
	std::unique_ptr<Actor> nelson;
	std::unique_ptr<ActorWithBankAccount> snake;
	std::unique_ptr<ActorWithBankAccount> smithers;
}; // TODO: использовать shared ptr ++

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
