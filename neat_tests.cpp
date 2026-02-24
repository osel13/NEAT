#include "neat_tests.h"

#include "genome.h"
#include "neural_network.h"

#include <cassert>
#include <iostream>

void neat_tests::mutation_add_neuron_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto neuron_count_before = network.neuron_count();
	const auto edge_count_before = network.edge_count();
	const auto disabled_before = network.disabled_edge_count();

	network.mutate_add_neuron();

	assert(network.neuron_count() == neuron_count_before + 1);
	assert(network.edge_count() == edge_count_before + 2);
	assert(network.disabled_edge_count() == disabled_before + 1);
	std::cout << "mutation_add_neuron_test passed" << std::endl;
}

void neat_tests::mutation_toggle_edge_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	assert(network.edge_count() == 1);
	assert(network.disabled_edge_count() == 0);

	network.mutate_toggle_edge();
	assert(network.disabled_edge_count() == 1);

	network.mutate_toggle_edge();
	assert(network.disabled_edge_count() == 0);
	std::cout << "mutation_toggle_edge_test passed" << std::endl;
}

void neat_tests::genome_mutation_stability_test()
{
	auto genome = neat::Genome(1, 1);
	for (int i = 0; i < 50; ++i) {
		genome.mutate();
		auto output = genome.evaluate({ 1.0f });
		assert(output.size() == 1);
	}
	std::cout << "genome_mutation_stability_test passed" << std::endl;
}
