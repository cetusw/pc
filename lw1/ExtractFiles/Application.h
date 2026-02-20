#pragma once

#include <string>

struct Config
{
	bool parallel = false;
	int maxProcesses = 1;
	std::string archiveName;
	std::string outputDirectory;
};

class Application
{
public:
	static void Run(int argc, char* argv[]);

private:
	static Config ParseArgs(int argc, char* argv[]);
};
