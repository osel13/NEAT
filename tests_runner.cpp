#include "tests_runner.h"

#include "neat_tests.h"
#include "neural_network_tests.h"

void tests_runner::run_all()
{
	neural_network::perceptron_test();
	neat::mutation_add_neuron_test();
	neat::mutation_toggle_edge_test();
	neat::genome_mutation_stability_test();
}
