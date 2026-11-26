#pragma once
#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class Homer final : public ActorWithBankAccount
{
public:
	explicit Homer(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};
