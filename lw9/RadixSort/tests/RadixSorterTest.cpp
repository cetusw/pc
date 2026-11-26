#include "OpenClContext.h"
#include "OpenClProgram.h"
#include "RadixSorter.h"
#include "utils/Utils.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <random>
#include <vector>

TEST(RadixSorter, SortsEmptyArray)
{
	Utils::ExpectSortedLikeStdSort({});
}

TEST(RadixSorter, SortsSingleElement)
{
	Utils::ExpectSortedLikeStdSort({ 1 });
}

TEST(RadixSorter, SortsSmallArray)
{
	Utils::ExpectSortedLikeStdSort({ 6, 3, 5, 1 });
}

TEST(RadixSorter, SortsArrayWithDuplicates)
{
	Utils::ExpectSortedLikeStdSort({ 5, 1, 5, 3, 1, 0, 5 });
}

TEST(RadixSorter, SortsAlreadySortedArray)
{
	Utils::ExpectSortedLikeStdSort({ 1, 2, 3, 4, 5, 6 });
}

TEST(RadixSorter, SortsReversedArray)
{
	Utils::ExpectSortedLikeStdSort({ 6, 5, 4, 3, 2, 1 });
}

TEST(RadixSorter, SortsArrayWithNegative)
{
	Utils::ExpectSortedLikeStdSort({ 6, 5, 4, 3, 2, 1 });
}

TEST(RadixSorter, SortsSignedArray)
{
	Utils::ExpectSortedLikeStdSort({ 5, -1, 0, -10, 3, INT32_MIN, INT32_MAX });
}

TEST(RadixSorter, SortsRandomArray)
{
	Utils::ExpectSortedLikeStdSort(Utils::GenerateRandomData(1000));
}

TEST(RadixSorter, SortsLargeRandomArray)
{
	Utils::ExpectSortedLikeStdSort(Utils::GenerateRandomData(100000));
}