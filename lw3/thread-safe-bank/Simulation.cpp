#include "Simulation.h"

#include "Actors/Apu.h"
#include "Actors/Bart.h"
#include "Actors/Homer.h"
#include "Actors/Liza.h"
#include "Actors/Marge.h"
#include "Actors/MrBurns.h"
#include "Actors/Nelson.h"
#include "Actors/Smithers.h"
#include "Actors/Snake.h"

#include <iostream>

Simulation::Simulation(const Money homerInitialCash, const Money margeInitialCash,
	const Money bartInitialCash, const Money lizaInitialCash, const Money apuInitialCash,
	const Money mrBurnsInitialCash, const Money nelsonInitialCash, const Money snakeInitialCash,
	const Money smithersInitialCash)
	: m_finalCash(0)
{
	m_initialCash = homerInitialCash + margeInitialCash + bartInitialCash + lizaInitialCash
		+ apuInitialCash + mrBurnsInitialCash + nelsonInitialCash + snakeInitialCash
		+ smithersInitialCash;
	m_bank = std::make_unique<Bank>(m_initialCash);

	m_actors.homer = std::make_shared<Homer>(homerInitialCash, NO_ACCOUNT, *m_bank);
	m_actors.marge = std::make_shared<Marge>(margeInitialCash, NO_ACCOUNT, *m_bank);
	m_actors.bart = std::make_shared<Bart>(bartInitialCash);
	m_actors.liza = std::make_shared<Liza>(lizaInitialCash);
	m_actors.apu = std::make_shared<Apu>(apuInitialCash, NO_ACCOUNT, *m_bank);
	m_actors.mrBurns = std::make_shared<MrBurns>(mrBurnsInitialCash, NO_ACCOUNT, *m_bank);
	m_actors.nelson = std::make_shared<Nelson>(nelsonInitialCash);
	m_actors.snake = std::make_shared<Snake>(snakeInitialCash, NO_ACCOUNT, *m_bank);
	m_actors.smithers = std::make_shared<Smithers>(smithersInitialCash, NO_ACCOUNT, *m_bank);

	m_allActors = { m_actors.homer, m_actors.marge, m_actors.bart, m_actors.liza,
		m_actors.apu, m_actors.mrBurns, m_actors.nelson, m_actors.snake, m_actors.smithers };

	m_actors.homer->OpenAccount();
	m_actors.marge->OpenAccount();
	m_actors.apu->OpenAccount();
	m_actors.mrBurns->OpenAccount();
	m_actors.snake->OpenAccount();
	m_actors.smithers->OpenAccount();
}

void Simulation::StartSimulation(const int iterationsCount)
{
	for (int i = 0; i < iterationsCount; i++)
	{
		for (const auto& actor : m_allActors)
		{
			actor->Act(m_actors);
		}
	}
	GetResult();
}

void Simulation::PrintResult() const
{
	std::cout << "Initial cash: " << m_initialCash << "\n"
			  << "Final money: " << m_finalCash << "\n";
}

void Simulation::GetResult()
{
	Money totalActorsCash = 0;
	for (const auto& actor : m_allActors)
	{
		totalActorsCash += actor->GetCash();
	}

	const Money totalActorsBalance = m_actors.homer->GetAccountBalance()
		+ m_actors.marge->GetAccountBalance() + m_actors.apu->GetAccountBalance()
		+ m_actors.mrBurns->GetAccountBalance() + m_actors.snake->GetAccountBalance()
		+ m_actors.smithers->GetAccountBalance();

	m_finalCash = totalActorsBalance + totalActorsCash;
}

Money Simulation::GetFinalCash() const { return m_finalCash; }