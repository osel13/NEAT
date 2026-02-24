#include "neural_network_tests.h"
#include "neural_network.h"

#include <cassert>
#include <iostream>

void neural_network::perceptron_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	auto output = network.evaluate({ 1 });
	assert(output.size() == 1);
	assert(output[0] > 0.0f && output[0] < 1.0f);
	std::cout << "perceptron_test passed" << std::endl;
}
