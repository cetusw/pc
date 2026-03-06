#include "Snake.h"
#include <iostream>

Snake::Snake(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}

void Snake::Act(Actors& actors)
{
	const Money stolenAmount = GenerateRandomInt(100);

	if (actors.homer->SendMoney(GetAccountId(), stolenAmount))
	{
		std::cout << "Snake stole money from Homer's bank account\n";
		if (SendCash(*actors.apu, stolenAmount))
		{
			std::cout << "Snake paid for Apu's products\n";
		}
	}
}


