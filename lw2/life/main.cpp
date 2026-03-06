#include "./Model/Board.h"
#include "Visualizer.h"
#include <iostream>
#include <stdexcept>
#include <string>

void PrintHelp()
{
	std::cout << "Usage:\n";
	std::cout << "  life generate OUTPUT_FILE WIDTH HEIGHT PROBABILITY\n";
	std::cout << "  life step INPUT_FILE NUM_THREADS [OUTPUT_FILE]\n";
	std::cout << "  life visualize INPUT_FILE NUM_THREADS\n";
}

void RunGenerate(const int argc, char** argv)
{
	if (argc != 6)
	{
		throw std::invalid_argument("Invalid arguments count for 'generate'. Expected 6.");
	}

	const std::string outFile = argv[2];
	const int width = std::stoi(argv[3]);
	const int height = std::stoi(argv[4]);
	const float prob = std::stof(argv[5]);

	Board board(width, height);
	board.GenerateCells(prob);
	Board::Save(board, outFile);

	std::cout << "Generated board " << width << "x" << height << " to " << outFile << "\n";
}

void RunStep(const int argc, char** argv)
{
	if (argc < 4 || argc > 5)
	{
		throw std::invalid_argument("Invalid arguments count for 'step'. Expected 4 or 5.");
	}

	const std::string inFile = argv[2];
	const int numThreads = std::stoi(argv[3]);
	const std::string outFile = (argc == 5) ? argv[4] : inFile;

	Board board = Board::Load(inFile);
	board.StartSynchronizer(numThreads);
	board.Step();
	Board::Save(board, outFile);

	std::cout << "Step computed in " << board.GetLastStepTime() << " ms\n";
}

void RunVisualize(int argc, char** argv);

int main(const int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			PrintHelp();
			return 1;
		}

		const std::string mode = argv[1];

		if (mode == "generate")
		{
			RunGenerate(argc, argv);
		}
		else if (mode == "step")
		{
			RunStep(argc, argv);
		}
		else if (mode == "visualize")
		{
			Visualizer visualizer;
			visualizer.Run(argc, argv);
		}
		else
		{
			PrintHelp();
			return 1;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}
	return 0;
}