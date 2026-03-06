#include "../Actor.h"
#include "../Simulation.h"
#include <gtest/gtest.h>

class Simulation;

void SimulationTest(const Money homerInitialCash, const Money margeInitialCash,
	const Money bartInitialCash, const Money lizaInitialCash, const Money apuInitialCash,
	const Money mrBurnsInitialCash, const Money nelsonInitialCash, const Money snakeInitialCash,
	const Money smithersInitialCash, const int iterationsCount)
{
	const Money initialCash = homerInitialCash + margeInitialCash + bartInitialCash
		+ lizaInitialCash + apuInitialCash + mrBurnsInitialCash + nelsonInitialCash
		+ snakeInitialCash + smithersInitialCash;

	Simulation simulation(homerInitialCash, margeInitialCash, bartInitialCash, lizaInitialCash,
		apuInitialCash, mrBurnsInitialCash, nelsonInitialCash, snakeInitialCash,
		smithersInitialCash);

	simulation.StartSimulation(iterationsCount);

	EXPECT_EQ(simulation.GetFinalCash(), initialCash);
}

TEST(SimulationTest, ZeroInitialCashAndIterations) { SimulationTest(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }

TEST(SimulationTest, ZeroInitialCash) { SimulationTest(0, 0, 0, 0, 0, 0, 0, 0, 0, 1); }

TEST(SimulationTest, ZeroIterations) { SimulationTest(1, 1, 1, 1, 1, 1, 1, 1, 1, 0); }

TEST(SimulationTest, LargeAmountOfIterations) { SimulationTest(1, 1, 1, 1, 1, 1, 1, 1, 1, 10000); }

TEST(SimulationTest, LargeAmountOfInitialCash)
{
	SimulationTest(100, 100, 100, 100, 100, 100, 100, 100, 100, 10);
}

TEST(SimulationTest, LargeAmountOfInitialCashAndIterations)
{
	SimulationTest(100, 100, 100, 100, 100, 100, 100, 100, 100, 100000);
}