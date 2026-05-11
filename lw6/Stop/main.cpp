#include "StopSource.h"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
	const StopSource source;
	StopToken token = source.GetToken();

	std::jthread worker([token] {
		while (!token.StopRequested())
		{
			std::cout << "Working...\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}

		std::cout << "Stopped\n";
	});

	std::this_thread::sleep_for(std::chrono::seconds(2));

	source.RequestStop();
}