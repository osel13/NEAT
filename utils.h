#pragma once

#include <random>
#include <iterator>

static std::mt19937 default_generator(std::random_device{}());

template<typename IteratorType, typename RandomGenerator>
IteratorType random_from_range(IteratorType from, IteratorType to, RandomGenerator& random) {
	auto distance = std::distance(from, to);
	std::uniform_int_distribution<> distribution(0, distance - 1);
	return *std::advance(from, distribution(random));
}

template<typename RealType, typename RandomGenerator> requires std::is_floating_point_v<RealType>
RealType random_from_range(RealType from, RealType to, RandomGenerator& random) {
	std::uniform_real_distribution<RealType> distribution(from, to);
	return distribution(random);
}

template<typename RealOrIteratorType>
RealOrIteratorType random_from_range(RealOrIteratorType start, RealOrIteratorType end) {
	return random_from_range(start, end, default_generator);
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
