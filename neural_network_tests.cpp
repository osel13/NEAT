#include "neural_network_tests.h"
#include "neural_network.h"

#include <algorithm>
#include <cassert>
#include <cmath>
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

void neural_network::add_edge_reenables_disabled_edge_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	network.mutate_toggle_edge();
	assert(network.disabled_edge_count() == static_cast<size_t>(1));

	for (int i = 0; i < 512 && network.disabled_edge_count() > 0; ++i) {
		network.add_edge();
	}

	assert(network.disabled_edge_count() == static_cast<size_t>(0));
}

void neural_network::add_edge_avoids_cycles_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto input_id = network.input_neuron_ids().front();
	const auto output_id = network.output_neuron_ids().front();

	for (int i = 0; i < 1024; ++i) {
		network.add_edge();
	}

	assert(network.has_edge(input_id, output_id));
	assert(!network.has_edge(output_id, input_id));
	assert(network.edge_count() == static_cast<size_t>(1));
}

void neural_network::mutate_change_weight_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto initial_weight_sum = network.edge_weight_sum();
	bool has_changed_weight = false;

	for (int i = 0; i < 128; ++i) {
		network.mutate_change_weight();
		const auto current_weight_sum = network.edge_weight_sum();
		if (std::fabs(current_weight_sum - initial_weight_sum) > 0.000001f) {
			has_changed_weight = true;
			break;
		}
	}

	assert(has_changed_weight);
}

void neural_network::network_metrics_test()
{
	auto network = neural_network::NeatNetwork(2, 1);

	assert(network.neuron_count() == static_cast<size_t>(3));
	assert(network.edge_count() == static_cast<size_t>(2));
	assert(network.disabled_edge_count() == static_cast<size_t>(0));

	const auto input_ids = network.input_neuron_ids();
	const auto output_ids = network.output_neuron_ids();
	assert(input_ids.size() == static_cast<size_t>(2));
	assert(output_ids.size() == static_cast<size_t>(1));

	for (const auto input_id : input_ids) {
		assert(network.has_edge(input_id, output_ids.front()));
	}

	const auto genes = network.edge_genes();
	assert(genes.size() == network.edge_count());
	assert(std::is_sorted(genes.begin(), genes.end(), [](const auto& left, const auto& right) {
		return left.innovation_id < right.innovation_id;
	}));
}

void neural_network::topology_diagram_test()
{
	auto network = neural_network::NeatNetwork(1, 1);
	const auto diagram = network.topology_diagram();
	assert(diagram.find("```mermaid") != std::string::npos);
	assert(diagram.find("graph LR") != std::string::npos);
	assert(diagram.find("==") != std::string::npos);

	network.mutate_toggle_edge();
	const auto hidden_disabled_edge_diagram = network.topology_diagram(true);
	assert(hidden_disabled_edge_diagram.find("~~~") != std::string::npos);

	const auto visible_disabled_edge_diagram = network.topology_diagram(false);
	assert(visible_disabled_edge_diagram.find("--") != std::string::npos);
}

void neural_network::crossover_io_shape_test()
{
	auto parent_a = neural_network::NeatNetwork(1, 1);
	auto parent_b = neural_network::NeatNetwork(1, 1);
	parent_b.mutate_add_neuron();

	auto child = parent_a.crossover(parent_b);
	assert(child.input_neuron_ids().size() == parent_a.input_neuron_ids().size());
	assert(child.output_neuron_ids().size() == parent_a.output_neuron_ids().size());

	const auto output = child.evaluate({ 1.0f });
	assert(output.size() == static_cast<size_t>(1));
}
