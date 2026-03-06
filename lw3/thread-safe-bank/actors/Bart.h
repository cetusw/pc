#ifndef BART_H
#define BART_H
#include "../Bank.h"
#include "../Actor.h"

class Bart final : public Actor
{
public:
	explicit Bart(Money cash);
	void Act(Actors& actors) override;
};



#endif //BART_H
