#include "Application.h"
#include <cstdlib>
#include <iostream>

int main(const int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage: ./BoundedBlockingQueue <producers> <consumers> <capacity>" << std::endl;
		return 1;
	}

	try
	{
		Application app(std::atoi(argv[1]), std::atoi(argv[2]), std::atoi(argv[3]));
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}