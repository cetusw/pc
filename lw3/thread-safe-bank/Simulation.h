#pragma once
#include "Actor.h"
#include <memory>
#include <vector>

struct Actors
{
	std::shared_ptr<ActorWithBankAccount> homer;
	std::shared_ptr<ActorWithBankAccount> marge;
	std::shared_ptr<Actor> bart;
	std::shared_ptr<Actor> liza;
	std::shared_ptr<ActorWithBankAccount> apu;
	std::shared_ptr<ActorWithBankAccount> mrBurns;
	std::shared_ptr<Actor> nelson;
	std::shared_ptr<ActorWithBankAccount> snake;
	std::shared_ptr<ActorWithBankAccount> smithers;
};

class Simulation
{
public:
	explicit Simulation(Money homerInitialCash, Money margeInitialCash, Money bartInitialCash,
		Money lizaInitialCash, Money apuInitialCash, Money mrBurnsInitialCash,
		Money nelsonInitialCash, Money snakeInitialCash, Money smithersInitialCash);

	void StartSimulation(int iterationsCount);
	void PrintResult() const;

	[[nodiscard]] Money GetFinalCash() const;

private:
	void GetResult();

	std::unique_ptr<Bank> m_bank;
	Actors m_actors{};
	std::vector<std::shared_ptr<Actor>> m_allActors;
	Money m_initialCash;
	Money m_finalCash;
};