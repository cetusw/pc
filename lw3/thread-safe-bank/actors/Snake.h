#ifndef SNAKE_H
#define SNAKE_H

#include "../ActorWithBankAccount.h"

class Snake final : public ActorWithBankAccount
{
public:
	explicit Snake(Money cash, AccountId accountId, Bank& bank);
	void Act(Actors& actors) override;
};



#endif //SNAKE_H
