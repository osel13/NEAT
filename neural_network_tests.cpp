#include "neural_network_tests.h"
#include "neural_network.h"

#include <iostream>

void neural_network::smallest_network_evaluation_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	auto output = network.evaluate({ 1 });
	for (const auto value : output) {
		std::cout << value << std::endl;
	}
}
