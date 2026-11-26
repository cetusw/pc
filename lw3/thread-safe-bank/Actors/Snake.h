#pragma once
#include "../ActorWithBankAccount.h"

class Snake final : public ActorWithBankAccount
{
public:
	explicit Snake(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};
