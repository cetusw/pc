#include "Application.h"

#include "Decompressor/IDecompressor.h"
#include "Decompressor/ParallelDecompressor.h"
#include "Decompressor/SequentialDecompressor.h"
#include "Extractor/Extractor.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>

void Application::Run(const int argc, char* argv[])
{
	Config cfg = ParseArgs(argc, argv);

	std::unique_ptr<IDecompressor> decompressor;
	if (cfg.parallel)
	{
		decompressor = std::make_unique<ParallelDecompressor>(cfg.maxProcesses);
	}
	else
	{
		decompressor = std::make_unique<SequentialDecompressor>();
	}

	const auto startTotal = std::chrono::high_resolution_clock::now();
	const auto extractedFiles = Extractor::Extract(cfg.archiveName, cfg.outputDirectory);

	const auto endSequential = std::chrono::high_resolution_clock::now();
	decompressor->Decompress(extractedFiles);

	const auto endTotal = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double> totalTime = endTotal - startTotal;
	const std::chrono::duration<double> sequentialTime = endSequential - startTotal;

	std::cout << "     Total time: " << totalTime.count() << "s\n";
	std::cout << "Sequential time: " << sequentialTime.count() << "s\n";
}

Config Application::ParseArgs(const int argc, char* argv[])
{
	Config cfg;
	if (argc < 3)
	{
		throw std::runtime_error("Invalid arguments count");
	}

	const std::string mode = argv[1];
	if (mode == "-S")
	{
		cfg.parallel = false;
		cfg.archiveName = argv[2];
		cfg.outputDirectory = argv[3];
	}
	else if (mode == "-P")
	{
		cfg.parallel = true;
		cfg.maxProcesses = std::stoi(argv[2]);
		cfg.archiveName = argv[3];
		cfg.outputDirectory = argv[4];
	}
	else
	{
		throw std::runtime_error("Unknown mode: " + mode);
	}

	return cfg;
}