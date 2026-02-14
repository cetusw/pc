#include "Application.h"

#include "Archiver.h"
#include "ICompressor.h"
#include "ParallelCompressor.h"
#include "SequentialCompressor.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>

void Application::Run(const int argc, char* argv[])
{
	Config cfg = ParseArgs(argc, argv);

	std::unique_ptr<ICompressor> compressor;
	if (cfg.parallel)
	{
		compressor = std::make_unique<ParallelCompressor>(cfg.maxProcesses);
	}
	else
	{
		compressor = std::make_unique<SequentialCompressor>();
	}

	const auto startTotal = std::chrono::high_resolution_clock::now();
	auto gzFiles = compressor->Compress(cfg.files);

	const auto startSequential = std::chrono::high_resolution_clock::now();
	Archiver::Archive(cfg.archiveName, gzFiles);

	const auto endTotal = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double> totalTime = endTotal - startTotal;
	const std::chrono::duration<double> sequentialTime = endTotal - startSequential;

	std::cout << "     Total time: " << totalTime.count() << "s\n";
	std::cout << "Sequential time: " << sequentialTime.count() << "s\n";
}

Config Application::ParseArgs(const int argc, char* argv[])
{
	Config cfg;
	if (argc < 4)
	{
		throw std::runtime_error("Invalid arguments count");
	}

	const std::string mode = argv[1];
	int fileStartIndex = 0;
	if (mode == "-S")
	{
		cfg.parallel = false;
		cfg.archiveName = argv[2];
		fileStartIndex = 3;
	}
	else if (mode == "-P")
	{
		cfg.parallel = true;
		cfg.maxProcesses = std::stoi(argv[2]);
		cfg.archiveName = argv[3];
		fileStartIndex = 4;
	}
	else
	{
		throw std::runtime_error("Unknown mode: " + mode);
	}

	for (int i = fileStartIndex; i < argc; ++i)
	{
		cfg.files.emplace_back(argv[i]);
	}

	return cfg;
}