#pragma once
#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class Marge final : public ActorWithBankAccount
{
public:
	explicit Marge(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};
