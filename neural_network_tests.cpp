#include "neural_network_tests.h"
#include "neural_network.h"

#include <cassert>
#include <vector>

void neural_network::perceptron_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto input_data = std::vector<float>{ 1.0f };
	const auto expected_output_size = static_cast<size_t>(1);
	const auto lower_bound = 0.0f;
	const auto upper_bound = 1.0f;

	const auto output = network.evaluate(input_data);
	const auto output_size = output.size();
	assert(output_size == expected_output_size);
	const auto output_value = output[0];
	assert(output_value > lower_bound && output_value < upper_bound);
}
