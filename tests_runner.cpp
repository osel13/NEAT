#include "tests_runner.h"

#include "neat_tests.h"
#include "neural_network_tests.h"

void tests_runner::run_all()
{
	neural_network::perceptron_test();
	neural_network::add_edge_reenables_disabled_edge_test();
	neural_network::add_edge_avoids_cycles_test();
	neural_network::mutate_change_weight_test();
	neural_network::network_metrics_test();
	neural_network::topology_diagram_test();
	neural_network::crossover_io_shape_test();
	neat::mutation_add_neuron_test();
	neat::mutation_toggle_edge_test();
	neat::genome_mutation_stability_test();
	neat::genome_api_test();
	neat::individual_api_test();
	neat::population_api_test();
	neat::evolution_runner_smoke_test();
}
