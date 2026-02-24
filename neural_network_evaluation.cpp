#include <cassert>

#include "neural_network.h"

namespace neural_network {
	void NeatNetwork::reset_intermediate_calculations()
	{
		for (auto& neuron_entry : neurons_by_id) {
			neuron_entry.second->reset();
		}
	}

	void NeatNetwork::collect_next_neurons(const std::shared_ptr<Neuron>& source_neuron, std::set<std::shared_ptr<Neuron>>& next_neurons) const
	{
		const auto outgoing_match = outgoing_edges_by_neuron_id.find(source_neuron->id);
		if (outgoing_match == outgoing_edges_by_neuron_id.end()) {
			return;
		}

		for (const auto* edge : outgoing_match->second) {
			const auto target_id = edge->to();
			if (is_output_neuron_id(target_id)) {
				continue;
			}

			auto next_neuron = get_neuron_by_id(target_id);
			if (next_neuron != nullptr) {
				next_neurons.insert(next_neuron);
			}
		}
	}

	void NeatNetwork::evaluate_neuron(const Neuron& neuron)
	{
		const auto source_output = neuron.evaluate();
		for (auto* edge : get_edges_from_neuron(neuron)) {
			auto target_neuron = get_neuron_by_id(edge->to());
			if (target_neuron == nullptr) {
				continue;
			}
			target_neuron->current_input += edge->evaluate(source_output);
		}
	}

	std::vector<float> NeatNetwork::evaluate(const std::vector<float>& input_data)
	{
		assert(input_data.size() >= input_neurons.size());
		for (size_t i = 0; i < input_neurons.size(); ++i) {
			input_neurons[i]->current_input = input_data[i];
		}

		auto current_neurons = std::set<std::shared_ptr<Neuron>>(input_neurons.begin(), input_neurons.end());
		while (!current_neurons.empty()) {
			auto next_neurons = std::set<std::shared_ptr<Neuron>>();
			for (const auto& neuron : current_neurons) {
				evaluate_neuron(*neuron);
				collect_next_neurons(neuron, next_neurons);
			}
			current_neurons = std::move(next_neurons);
		}

		auto results = std::vector<float>(output_neurons.size());
		for (size_t i = 0; i < output_neurons.size(); ++i) {
			results[i] = output_neurons[i]->evaluate();
		}

		reset_intermediate_calculations();
		return results;
	}
} // namespace neural_network
