#ifndef LIZA_H
#define LIZA_H

#include "../Actor.h"
#include "../Bank.h"

class Liza final : public Actor
{
public:
	explicit Liza(Money cash);
	void Act(Actors& actors) override;
};

#endif // LIZA_H
