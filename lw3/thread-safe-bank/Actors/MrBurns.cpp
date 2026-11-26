#include "MrBurns.h"
#include <iostream>

MrBurns::MrBurns(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}
void MrBurns::Act(Actors& actors)
{
	if (SendMoney(actors.homer->GetAccountId(), 500))
	{
		std::cout << "Mr. Burns paid Homer for his work\n";
	}
	if (GenerateRandomInt(1))
	{
		if (SendMoney(actors.smithers->GetAccountId(), 500))
		{
			std::cout << "Mr. Burns paid Smithers for his work\n";
		}
	}
	else
	{
		std::cout << "Mr. Burns paid Smithers for his work on wrong account id\n";
	}
}
