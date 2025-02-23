#pragma once

#include <random>
#include <iterator>

static std::mt19937 default_generator(std::random_device{}());

template<typename Iterator, typename RandomGenerator>
Iterator random_from_range(Iterator start, Iterator end, RandomGenerator& random) {
	auto distance = std::distance(start, end);
    std::uniform_int_distribution<> distribution(0, std::distance(start, end) - 1);
    return *std::advance(start, distribution(random));
}

template <typename Type, typename RandomGenerator>
const Type& random_thresholded_pick(float threshold, const Type& a, const Type& b, RandomGenerator& random) {
    std::uniform_int_distribution<> distribution(0, 1);
	const auto random_value = distribution(random);
    return random_value ? a : b;
}

template <typename Type>
const Type& random_thresholded_pick(float threshold, const Type& a, const Type& b) {
	return random_thresholded_pick(threshold, a, b, default_generator);
}
