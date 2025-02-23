#pragma once

#include <iostream>
#include "activation_function.hpp"

namespace neural_network {
	class Neuron {
		int id;
		float bias;
		float current_input;
		ActivationFunction activation_function;

		void reset();
		friend class NeatNetwork;
	public:
		Neuron(unsigned innovation_number);

		float evaluate() const;
		Neuron crossover(const Neuron& other) const;

		std::ostream& operator<<(std::ostream& stream) const;
		std::istream& operator>>(std::istream& stream);
	};
} // namespace neural_network