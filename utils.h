#pragma once

#include <random>
#include <iterator>
#include <type_traits>

static std::mt19937 default_generator(std::random_device{}());

template<typename IteratorType, typename RandomGenerator>
auto random_from_range(IteratorType from, IteratorType to, RandomGenerator& random) -> std::enable_if_t<!std::is_floating_point<IteratorType>::value, IteratorType> {
	using DistanceType = typename std::iterator_traits<IteratorType>::difference_type;
	const DistanceType distance = std::distance(from, to);
	std::uniform_int_distribution<DistanceType> distribution(0, distance - 1);
	auto advance_by = distribution(random);
	auto result = from;
	std::advance(result, advance_by);
	return result;
}

template<typename RealType, typename RandomGenerator>
auto random_from_range(RealType from, RealType to, RandomGenerator& random) -> std::enable_if_t<std::is_floating_point<RealType>::value, RealType> {
	std::uniform_real_distribution<RealType> distribution(from, to);
	return distribution(random);
}

template<typename RealOrIteratorType>
RealOrIteratorType random_from_range(RealOrIteratorType start, RealOrIteratorType end) {
	return random_from_range(start, end, default_generator);
}

template<typename Container, typename RandomGenerator>
auto random_element(Container& container, RandomGenerator& random) -> decltype(*container.begin()) {
	return *random_from_range(container.begin(), container.end(), random);
}

template<typename Container>
auto random_element(Container& container) -> decltype(*container.begin()) {
	return random_element(container, default_generator);
}

template<typename Container, typename RandomGenerator>
auto random_element(const Container& container, RandomGenerator& random) -> decltype(*container.begin()) {
	return *random_from_range(container.begin(), container.end(), random);
}

template<typename Container>
auto random_element(const Container& container) -> decltype(*container.begin()) {
	return random_element(container, default_generator);
}

template <typename Type, typename RandomGenerator>
const Type& random_thresholded_pick(float threshold, const Type& a, const Type& b, RandomGenerator& random) {
	std::bernoulli_distribution distribution(threshold);
	const auto is_first_selected = distribution(random);
	return is_first_selected ? a : b;
}

template <typename Type>
const Type& random_thresholded_pick(float threshold, const Type& a, const Type& b) {
	return random_thresholded_pick(threshold, a, b, default_generator);
}
