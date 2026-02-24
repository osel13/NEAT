#include "neat_tests.h"

#include "genome.h"
#include "neural_network.h"

#include <cassert>
#include <vector>

void neat::mutation_add_neuron_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto neuron_count_before = network.neuron_count();
	const auto edge_count_before = network.edge_count();
	const auto disabled_before = network.disabled_edge_count();
	const auto expected_neuron_count = neuron_count_before + 1;
	const auto expected_edge_count = edge_count_before + 2;
	const auto expected_disabled_count = disabled_before + 1;

	network.mutate_add_neuron();

	const auto neuron_count_after = network.neuron_count();
	const auto edge_count_after = network.edge_count();
	const auto disabled_after = network.disabled_edge_count();

	assert(neuron_count_after == expected_neuron_count);
	assert(edge_count_after == expected_edge_count);
	assert(disabled_after == expected_disabled_count);
}

void neat::mutation_toggle_edge_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto expected_edge_count = static_cast<size_t>(1);
	const auto expected_disabled_before = static_cast<size_t>(0);
	const auto expected_disabled_after_toggle = static_cast<size_t>(1);

	const auto edge_count_before = network.edge_count();
	const auto disabled_before = network.disabled_edge_count();
	assert(edge_count_before == expected_edge_count);
	assert(disabled_before == expected_disabled_before);

	network.mutate_toggle_edge();
	const auto disabled_after_first_toggle = network.disabled_edge_count();
	assert(disabled_after_first_toggle == expected_disabled_after_toggle);

	network.mutate_toggle_edge();
	const auto disabled_after_second_toggle = network.disabled_edge_count();
	assert(disabled_after_second_toggle == expected_disabled_before);
}

void neat::genome_mutation_stability_test()
{
	auto genome = neat::Genome(1, 1);
	const auto expected_output_size = static_cast<size_t>(1);
	const auto input_data = std::vector<float>{ 1.0f };
	for (int i = 0; i < 50; ++i) {
		genome.mutate();
		const auto output = genome.evaluate(input_data);
		const auto output_size = output.size();
		assert(output_size == expected_output_size);
	}
}
