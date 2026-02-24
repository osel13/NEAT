#include "neural_network.h"

namespace neural_network {
	NeatNetwork NeatNetwork::crossover(const NeatNetwork& other) const
	{
		NeatNetwork result;
		crossover_neurons(result, other);
		crossover_edges(result, other);
		result.rebuild_outgoing_edges();
		result.update_next_edge_innovation_number();
		return result;
	}

	void NeatNetwork::crossover_neurons(NeatNetwork& result, const NeatNetwork& other) const
	{
		for (const auto& neuron_entry : neurons_by_id) {
			const auto neuron_id = neuron_entry.first;
			const auto& neuron = neuron_entry.second;
			auto other_neuron = other.get_neuron_by_id(neuron_id);
			if (other_neuron != nullptr) {
				auto new_neuron = std::make_shared<Neuron>(neuron->crossover(*other_neuron));
				result.add_neuron(new_neuron, *this, neuron);
				continue;
			}

			result.add_neuron(std::make_shared<Neuron>(*neuron), *this, neuron);
		}
	}

	void NeatNetwork::crossover_edges(NeatNetwork& result, const NeatNetwork& other) const
	{
		for (const auto& edge_entry : edges_by_id) {
			const auto& edge = edge_entry.second;
			const auto* matching_edge = other.get_edge_by_innovation_id(edge->innovation_id);
			if (matching_edge != nullptr) {
				result.insert_edge(std::make_unique<Edge>(edge->crossover(*matching_edge)));
				continue;
			}

			result.insert_edge(std::make_unique<Edge>(*edge));
		}
	}
} // namespace neural_network
