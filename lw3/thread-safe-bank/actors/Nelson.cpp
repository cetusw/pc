#include "Nelson.h"

#include <iostream>

Nelson::Nelson(const Money cash)
	: Actor(cash)
{
}
void Nelson::Act(Actors& actors)
{
	const Money stolenAmount = GenerateRandomInt(20);

	if (actors.bart->SendCash(*this, stolenAmount))
	{
		std::cout << "Nelson stole Bart's cash\n";
		if (SendCash(*actors.apu, stolenAmount))
		{
			std::cout << "Nelson paid for Apu's cigarettes\n";
		}
	}
}




