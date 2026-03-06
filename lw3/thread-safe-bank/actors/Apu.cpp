#include "Apu.h"
#include <iostream>

Apu::Apu(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}
void Apu::Act(Actors& actors)
{
	if (SendMoney(actors.mrBurns->GetAccountId(), 100))
	{
		std::cout << "Apu paid for electricity\n";
	}
	if (GetCash() > 0)
	{
		if (DepositMoney(GetCash()))
		{
			std::cout << "Apu deposited money to his bank account\n";
		}
	}
}
