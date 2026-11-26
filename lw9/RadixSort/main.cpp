#include "OpenClContext.h"
#include "OpenClProgram.h"
#include "RadixSorter.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <vector>

void PrintVector(const char* title, const std::vector<std::uint32_t>& values)
{
	std::cout << title;

	for (const auto value : values)
	{
		std::cout << value << ' ';
	}

	std::cout << '\n';
}

int main()
{
	try
	{
		OpenClContext openCl;
		const OpenClProgram program(
			openCl.GetContext(),
			openCl.GetDevice(),
			"../kernels/radix_sort.cl");

		RadixSorter sorter(openCl, program);

		const std::vector<std::uint32_t> input = {6, 3, 5, 1};
		const auto output = sorter.Sort(input);

		PrintVector("input:  ", input);
		PrintVector("output: ", output);

		const auto isZero = sorter.BuildZeroMask(input, 0);

		PrintVector("input:  ", input);
		PrintVector("isZero: ", isZero);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}

	return 0;
}