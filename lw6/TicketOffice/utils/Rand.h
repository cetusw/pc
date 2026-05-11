#pragma once
#include <concepts>
#include <random>

namespace Rand
{
template <std::integral T>
static T Get(T min, T max)
{
	thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<T> distribution(min, max);
	return distribution(generator);
}
} // namespace Rand
