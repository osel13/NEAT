#include "activation_function.hpp"

#include <cmath>

float ActivationFunction::evaluate(const float input) const
{
	// Sigmoid activation function
	return 1.0 / (1.0 + exp(-input));
}
