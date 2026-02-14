#include "Application.h"
#include <iostream>

int main(const int argc, char* argv[])
{
	try
	{
		Application::Run(argc, argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}