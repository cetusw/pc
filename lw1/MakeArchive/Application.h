#ifndef MAKEARCHIVE_APPLICATION_H
#define MAKEARCHIVE_APPLICATION_H

#include <string>
#include <vector>

struct Config
{
	bool parallel = false;
	int maxProcesses = 1;
	std::string archiveName;
	std::vector<std::string> files;
};

class Application
{
public:
	static void Run(int argc, char* argv[]);

private:
	static Config ParseArgs(int argc, char* argv[]);
};

#endif // MAKEARCHIVE_APPLICATION_H
