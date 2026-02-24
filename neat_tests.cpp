#include "neat_tests.h"

#include "genome.h"
#include "individual.h"
#include "neat.h"
#include "neural_network.h"
#include "population.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

namespace {
	size_t line_count(const std::string& text)
	{
		return std::count(text.begin(), text.end(), '\n');
	}
}

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

void neat::genome_api_test()
{
	auto genome = neat::Genome(1, 1);

	const auto neuron_count_before = genome.get_network().neuron_count();
	const auto edge_count_before = genome.get_network().edge_count();
	const auto disabled_before = genome.get_network().disabled_edge_count();
	genome.mutate_add_neuron();
	assert(genome.get_network().neuron_count() == neuron_count_before + 1);
	assert(genome.get_network().edge_count() == edge_count_before + 2);
	assert(genome.get_network().disabled_edge_count() == disabled_before + 1);

	const auto disabled_before_toggle = genome.get_network().disabled_edge_count();
	genome.mutate_toggle_edge();
	assert(genome.get_network().disabled_edge_count() != disabled_before_toggle);

	const auto weight_before = genome.get_network().edge_weight_sum();
	bool has_changed_weight = false;
	for (int i = 0; i < 128; ++i) {
		genome.mutate_change_weight();
		if (std::fabs(genome.get_network().edge_weight_sum() - weight_before) > 0.000001f) {
			has_changed_weight = true;
			break;
		}
	}
	assert(has_changed_weight);

	auto other = neat::Genome(1, 1);
	other.mutate_add_neuron();
	auto child = genome.crossover(other);
	const auto child_output = child.evaluate({ 1.0f });
	assert(child_output.size() == static_cast<size_t>(1));
}

void neat::individual_api_test()
{
	auto individual = neat::Individual(neat::Genome(1, 1));
	individual.evaluate_fitness();
	const auto fitness = individual.get_fitness();
	assert(fitness >= 0.0f);
	assert(fitness <= 2.0f);
	assert(individual.get_genome().evaluate({ 1.0f }).size() == static_cast<size_t>(1));

	auto other = neat::Individual(neat::Genome(1, 1));
	auto child = individual.crossover(other);
	child.evaluate_fitness();
	assert(child.get_fitness() >= 0.0f);
	assert(child.get_fitness() <= 2.0f);
}

void neat::population_api_test()
{
	constexpr int population_size = 6;
	auto population = neat::Population(population_size, 0.5f);
	population.evaluate_fitness();

	auto output = std::ostringstream();
	population.print(output);
	assert(line_count(output.str()) == static_cast<size_t>(population_size));

	population.select();
	auto selected_output = std::ostringstream();
	population.print(selected_output);
	assert(line_count(selected_output.str()) == static_cast<size_t>(population_size / 2));

	population.speciate();
	population.crossover();
	population.mutate();
	population.evaluate_fitness();
	auto crossed_output = std::ostringstream();
	population.print(crossed_output);
	assert(line_count(crossed_output.str()) == static_cast<size_t>(population_size));

	auto topology_output = std::ostringstream();
	population.print_topologies(3, topology_output);
	assert(topology_output.str().find("Topologies (top 3)") != std::string::npos);

	auto next_generation_population = neat::Population(population_size, 0.5f);
	next_generation_population.next_generation();
	auto next_generation_output = std::ostringstream();
	next_generation_population.print(next_generation_output);
	assert(line_count(next_generation_output.str()) == static_cast<size_t>(population_size));
}

void neat::evolution_runner_smoke_test()
{
	auto output = std::ostringstream();
	auto runner = neat::EvolutionRunner(6, 0.5f, output);
	runner.run_evolution(2);

	const auto output_text = output.str();
	assert(output_text.find("Topologies (top 3)") != std::string::npos);
	assert(output_text.find("```mermaid") != std::string::npos);
}
