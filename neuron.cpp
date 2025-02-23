#include <cassert>
#include "neuron.h"
#include "utils.h"

namespace neural_network {
	void Neuron::reset()
	{
		current_input = 0;
	}

	Neuron::Neuron(unsigned innovation_id)
		: id(innovation_id)
		, bias(0)
		, activation_function()
		, current_input(0)
	{
	}

	float Neuron::evaluate() const
	{
		return activation_function.evaluate(current_input + bias);
	}

	Neuron Neuron::crossover(const Neuron& other) const
	{
		assert(id == other.id);
		auto result = Neuron(id);
		const auto threshold = 0.5f;
		result.bias = random_thresholded_pick(threshold, bias, other.bias);
		result.activation_function = random_thresholded_pick(threshold, activation_function, other.activation_function);
		return result;
	}
	std::ostream& Neuron::operator<<(std::ostream& stream) const
	{
		const auto separator = ' ';
		return stream << id << separator << bias << separator;
	}
	std::istream& Neuron::operator>>(std::istream& stream)
	{
		return stream >> id >> std::ws >> bias;
	}
} // namespace neural_network