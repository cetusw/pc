#include "Bart.h"
#include <iostream>
#include "../ActorWithBankAccount.h"

Bart::Bart(const Money cash)
	: Actor(cash)
{
}
void Bart::Act(Actors& actors)
{
	if (SendCash(*actors.apu, 1))
	{
		std::cout << "Bart paid for Apu's products\n";
	}
}


