#include "activation_function.hpp"

#include <cmath>

float ActivationFunction::evaluate(const float input) const
{
	// Sigmoid activation function
	return 1.0f / (1.0f + exp(-input));
}
