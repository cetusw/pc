#pragma once
#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class Smithers final : public ActorWithBankAccount
{
public:
	explicit Smithers(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};
