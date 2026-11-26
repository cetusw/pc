#pragma once

#include "RadixSorter.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <random>

namespace Utils
{
inline RadixSorter CreateSorter(OpenClContext& openCl)
{
	const OpenClProgram program(
		openCl.GetContext(),
		openCl.GetDevice(),
		"../kernels/radix_sort.cl");

	return { openCl, program };
}

inline std::vector<std::int32_t> GenerateRandomData(const std::size_t size)
{
	std::mt19937 generator;
	std::uniform_int_distribution distribution;

	std::vector<std::int32_t> data(size);

	for (auto& value : data)
	{
		value = distribution(generator);
	}

	return data;
}

inline void ExpectSortedLikeStdSort(const std::vector<std::int32_t>& input)
{
	OpenClContext openCl;
	auto sorter = CreateSorter(openCl);

	auto expected = input;
	std::ranges::sort(expected);

	const auto actual = sorter.Sort(input);

	EXPECT_EQ(actual, expected);
}
} // namespace

