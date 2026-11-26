#pragma once
#include "../Actor.h"

class Nelson final : public Actor
{
public:
	explicit Nelson(Money cash);
	void Act(Actors& actors) override;
};
