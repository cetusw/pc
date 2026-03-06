#include "Liza.h"
#include "../ActorWithBankAccount.h"
#include <iostream>

Liza::Liza(const Money cash)
	: Actor(cash)
{
}
void Liza::Act(Actors& actors)
{
	if (SendCash(*actors.apu, 1))
	{
		std::cout << "Lisa paid for Apu's products\n";
	}
}
