#include "Simulation.h"
#include <iostream>

Simulation::Simulation(const Money homerInitialCash, const Money margeInitialCash,
	const Money bartInitialCash, const Money lizaInitialCash, const Money apuInitialCash,
	const Money mrBurnsInitialCash, const Money nelsonInitialCash, const Money snakeInitialCash,
	const Money smithersInitialCash)
	: m_initialCash(homerInitialCash + margeInitialCash + bartInitialCash + lizaInitialCash
		  + apuInitialCash + mrBurnsInitialCash + nelsonInitialCash + snakeInitialCash
		  + smithersInitialCash)
	, m_finalCash(0)
{
	m_bank = std::make_unique<Bank>(m_initialCash);

	m_homer = std::make_unique<Homer>(homerInitialCash, NO_ACCOUNT, *m_bank);
	m_marge = std::make_unique<Marge>(margeInitialCash, NO_ACCOUNT, *m_bank);
	m_bart = std::make_unique<Bart>(bartInitialCash);
	m_liza = std::make_unique<Liza>(lizaInitialCash);
	m_apu = std::make_unique<Apu>(apuInitialCash, NO_ACCOUNT, *m_bank);
	m_mrBurns = std::make_unique<MrBurns>(mrBurnsInitialCash, NO_ACCOUNT, *m_bank);
	m_nelson = std::make_unique<Nelson>(nelsonInitialCash);
	m_snake = std::make_unique<Snake>(snakeInitialCash, NO_ACCOUNT, *m_bank);
	m_smithers = std::make_unique<Smithers>(smithersInitialCash, NO_ACCOUNT, *m_bank);

	m_homer->OpenAccount();
	m_marge->OpenAccount();
	m_apu->OpenAccount();
	m_mrBurns->OpenAccount();
	m_snake->OpenAccount();
	m_smithers->OpenAccount();

	m_actors.homer = std::move(m_homer);
	m_actors.marge = std::move(m_marge);
	m_actors.bart = std::move(m_bart);
	m_actors.liza = std::move(m_liza);
	m_actors.apu = std::move(m_apu);
	m_actors.mrBurns = std::move(m_mrBurns);
	m_actors.nelson = std::move(m_nelson);
	m_actors.snake = std::move(m_snake);
	m_actors.smithers = std::move(m_smithers);
}

void Simulation::StartSimulation(const int iterationsCount)
{
	for (int i = 0; i < iterationsCount; i++)
	{
		m_actors.homer->Act(m_actors);
		m_actors.marge->Act(m_actors);
		m_actors.bart->Act(m_actors);
		m_actors.liza->Act(m_actors);
		m_actors.apu->Act(m_actors);
		m_actors.mrBurns->Act(m_actors);
		m_actors.nelson->Act(m_actors);
		m_actors.snake->Act(m_actors);
		m_actors.smithers->Act(m_actors);
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
	const Money totalActorsCash = m_actors.homer->GetCash() + m_actors.marge->GetCash()
		+ m_actors.bart->GetCash() + m_actors.liza->GetCash() + m_actors.apu->GetCash()
		+ m_actors.mrBurns->GetCash() + m_actors.nelson->GetCash() + m_actors.snake->GetCash()
		+ m_actors.smithers->GetCash();

	const Money totalActorsBalance = m_actors.homer->GetAccountBalance()
		+ m_actors.marge->GetAccountBalance() + m_actors.apu->GetAccountBalance()
		+ m_actors.mrBurns->GetAccountBalance() + m_actors.snake->GetAccountBalance()
		+ m_actors.smithers->GetAccountBalance();

	m_finalCash = totalActorsBalance + totalActorsCash;
}

Money Simulation::GetFinalCash() const { return m_finalCash; }