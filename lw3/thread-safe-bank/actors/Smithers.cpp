#include "Smithers.h"
#include <iostream>

Smithers::Smithers(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}

void Smithers::Act(Actors& actors)
{
	if (SendMoney(actors.apu->GetAccountId(), 20))
	{
		std::cout << "Smithers paid for Apu's products\n";
	}

	if (GenerateRandomInt(1))
	{
		const auto cash = CloseAccount();
		OpenAccount();
		if (DepositMoney(cash))
		{
			std::cout << "Smithers reopened his bank account\n";
		}
	}
}
