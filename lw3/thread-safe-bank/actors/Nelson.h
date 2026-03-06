#ifndef NELSON_H
#define NELSON_H

#include "../Actor.h"
#include "../ActorWithBankAccount.h"

class Nelson final : public Actor
{
public:
	explicit Nelson(Money cash);
	void Act(Actors& actors) override;
};

#endif // NELSON_H
