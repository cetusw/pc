#include "Simulation.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main(const int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			throw std::invalid_argument("Invalid arguments count\n");
		}
		Simulation simulation(100, 100, 100, 100, 100, 100, 100, 100, 100);
		const int iterationsCount = std::stoi(argv[1]);
		simulation.StartSimulation(iterationsCount);
		simulation.PrintResult();
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << "\n";
		return 1;
	}

	return 0;
}