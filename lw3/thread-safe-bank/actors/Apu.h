#ifndef APU_H
#define APU_H

#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class Apu final : public ActorWithBankAccount
{
public:
	explicit Apu(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};

#endif // APU_H
