
#include "neural_network_tests.h"
#include "neat_tests.h"
#include "population.h"
#include <iostream>

int main(int argc, char* argv[]) {
	neural_network::perceptron_test();
	neat_tests::mutation_add_neuron_test();
	neat_tests::mutation_toggle_edge_test();
	neat_tests::genome_mutation_stability_test();
	neat::Population population(100, 0.5);
	population.crossover();
	population.mutate();
	population.print();
	return 0;
}
