#include <cassert>
#include "edge.h"
#include "utils.h"

namespace neural_network {
	Edge::Edge(unsigned from_id, unsigned to_id, unsigned innovation_id)
		: id(from_id, to_id)
		, weight(random_from_range(-1.f, 1.f))
		, is_disabled(false)
		, innovation_id(innovation_id)
	{
	}

	bool Edge::operator==(const Edge& other) const
	{
		return id == other.id;
	}

	std::size_t Edge::operator()() const
	{
		return id();
	}

	void Edge::set_disabled(const bool value)
	{
		is_disabled = value;
	}

	float Edge::evaluate(const float input) const
	{
		if (is_disabled) {
			return 0;
		}
		return input * weight;
	}

	Edge Edge::crossover(const Edge& other) const
	{
		assert(innovation_id == other.innovation_id);
		auto result = Edge(*this);
		const auto threshold = 0.5;
		result.weight = random_thresholded_pick(threshold, weight, other.weight);
		result.is_disabled = is_disabled || other.is_disabled;
		return result;
	}
} // namespace neural_network
