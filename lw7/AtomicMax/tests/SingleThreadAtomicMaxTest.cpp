#include "../AtomicMax.h"

#include <gtest/gtest.h>

TEST(AtomicMax, GetValue)
{
	const AtomicMax maxValue(10);

	EXPECT_EQ(maxValue.GetValue(), 10);
}

TEST(AtomicMax, UpdatePositiveValueWithHigherPositive)
{
	AtomicMax maxValue(10);

	maxValue.Update(20);

	EXPECT_EQ(maxValue.GetValue(), 20);
}

TEST(AtomicMax, UpdatePositiveValueWithLowerPositive)
{
	AtomicMax maxValue(10);

	maxValue.Update(5);

	EXPECT_EQ(maxValue.GetValue(), 10);
}

TEST(AtomicMax, UpdateNegativeValueWithHigherNegative)
{
	AtomicMax maxValue(-10);

	maxValue.Update(-5);

	EXPECT_EQ(maxValue.GetValue(), -5);
}

TEST(AtomicMax, UpdateNegativeValueWithLowerNegative)
{
	AtomicMax maxValue(-10);

	maxValue.Update(-20);

	EXPECT_EQ(maxValue.GetValue(), -10);
}

TEST(AtomicMax, UpdatePositiveValueWithNegative)
{
	AtomicMax maxValue(10);

	maxValue.Update(-10);

	EXPECT_EQ(maxValue.GetValue(), 10);
}

TEST(AtomicMax, UpdateValueWithTheSameValue)
{
	AtomicMax maxValue(10);

	maxValue.Update(10);

	EXPECT_EQ(maxValue.GetValue(), 10);
}

TEST(AtomicMax, UpdateValueWithTheSameValueZero)
{
	AtomicMax maxValue(0);

	maxValue.Update(0);

	EXPECT_EQ(maxValue.GetValue(), 0);
}

TEST(AtomicMax, UpdateValueWithHigherZero)
{
	AtomicMax maxValue(-10);

	maxValue.Update(0);

	EXPECT_EQ(maxValue.GetValue(), 0);
}

TEST(AtomicMax, UpdateValueWithLowerZero)
{
	AtomicMax maxValue(10);

	maxValue.Update(0);

	EXPECT_EQ(maxValue.GetValue(), 10);
}