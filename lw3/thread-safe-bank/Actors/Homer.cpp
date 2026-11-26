#include "Homer.h"
#include <iostream>

Homer::Homer(const Money cash, const AccountId accountId, Bank& bank)
	: ActorWithBankAccount(cash, accountId, bank)
{
}

void Homer::Act(Actors& actors)
{
	if (SendMoney(actors.marge->GetAccountId(), 10))
	{
		std::cout << "Homer sent money to Marge's card\n";
	}
	if (SendMoney(actors.mrBurns->GetAccountId(), 100))
	{
		std::cout << "Homer paid for electricity\n";
	}
	if (WithdrawMoney(5))
	{
		if (SendCash(*actors.bart, 5))
		{
			std::cout << "Homer gave cash to Bart\n";
		}
	}
	if (WithdrawMoney(5))
	{
		if (SendCash(*actors.liza, 5))
		{
			std::cout << "Homer gave cash to Lisa\n";
		}
	}
}