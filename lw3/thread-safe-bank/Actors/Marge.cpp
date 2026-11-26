#include "Marge.h"
#include <iostream>

Marge::Marge(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}

void Marge::Act(Actors& actors)
{
	if (SendMoney(actors.apu->GetAccountId(), 5))
	{
		std::cout << "Marge paid for Apu's products\n";
	}
}