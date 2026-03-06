#ifndef MRBURNS_H
#define MRBURNS_H

#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class MrBurns final : public ActorWithBankAccount
{
public:
	explicit MrBurns(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};


#endif //MRBURNS_H
