#include <set>
#include <map>
#include "neural_network.h"
#include "utils.h"

namespace neural_network {
	int NeatNetwork::next_id = 0;

	void NeatNetwork::mutate_add_neuron()
	{
		if (edges.empty()) {
			// no edges to split
			return;
		}
		// TODO: implement
		//auto& edge = random_select(edges);
	}

	void NeatNetwork::add_neuron(const SharedNeuronReference& new_neuron, const NeatNetwork& old_network, const SharedNeuronReference& old_neuron) {
		if (std::find(old_network.input_neurons.begin(), old_network.input_neurons.end(), old_neuron) != old_network.input_neurons.end()) {
			input_neurons.emplace_back(new_neuron);
		}
		if (std::find(old_network.output_neurons.begin(), old_network.output_neurons.end(), old_neuron) != old_network.output_neurons.end()) {
			output_neurons.emplace_back(new_neuron);
		}
		neurons.emplace_back(new_neuron);
	}

	NeatNetwork NeatNetwork::crossover(const NeatNetwork& other) const
	{
		// We assume that this network is dominant over the other
		// Take the dominant neurons and crossover them with recessive neuron counterparts, if they exist
		assert(id != other.id);
		NeatNetwork result;
		const auto threshold = random_from_range(0.1f, 0.9f);
		crossover_neurons(result, other);
		crossover_edges(result, other);
		return result;
	}

	void NeatNetwork::reset_intermediate_calculations()
	{
		for (auto& neuron : neurons) {
			neuron->reset();
		}
	}

	NeatNetwork::NeatNetwork()
		: id(NeatNetwork::next_id++)
		, fitness(0)
		, next_edge_innovation_number(0)
		, next_neuron_innovation_number(0)
	{
	}

	NeatNetwork::NeatNetwork(unsigned input_neuron_count, unsigned output_neuron_count)
		: NeatNetwork()
	{
		create_special_neuron(input_neurons, input_neuron_count);
		create_special_neuron(output_neurons, output_neuron_count);
		for (const auto& input_neuron : input_neurons) {
			for (const auto& output_neuron : output_neurons) {
				edges.emplace(Edge(input_neuron->id, output_neuron->id, next_edge_innovation_number++));
			}
		}
	}

	void NeatNetwork::evaluate_neuron(const Neuron& neuron) {
		auto edge_it = std::find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
			return edge.id.from_id == neuron.id;
		});
		auto neuron_it = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
			return edge_it->id.to_id == neuron->id;
		});
		(*neuron_it)->current_input += edge_it->evaluate(neuron.evaluate());
	}

	void NeatNetwork::evaluate_layer(const Neurons& layer)
	{
		for (const auto& neuron : layer) {
			evaluate_neuron(*neuron);
		}
	}

	void NeatNetwork::create_special_neuron(SharedNeurons& special_neuron_container, unsigned int count)
	{
		for (unsigned i = 0; i < count; i++) {
			auto neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
			special_neuron_container.emplace_back(neuron);
			neurons.emplace_back(neuron);
		}
	}

	void NeatNetwork::crossover_neurons(NeatNetwork& result, const NeatNetwork& other) const
	{
		for (unsigned i = 0; i < neurons.size(); ++i) {
			const auto& neuron = neurons[i];
			const auto& other_neuron = other.neurons[i];
			if (neuron->id == other_neuron->id) {
				auto new_neuron = std::make_shared<Neuron>(neuron->crossover(*other_neuron));
				result.add_neuron(new_neuron, *this, neuron);
				continue;
			}
			result.add_neuron(neuron, *this, neuron);
		}
	}

	void NeatNetwork::crossover_edges(NeatNetwork& result, const NeatNetwork& other) const
	{
		for (const auto& edge : edges) {
			const auto& other_edge = other.edges.find(edge);
			if (other_edge != other.edges.end()) {
				result.edges.emplace(edge.crossover(*other_edge));
				continue;
			}
			result.edges.emplace(edge);
		}
		return;
		//

		/*for (unsigned i = 0; i < std::max(edges.size(), other.edges.size()); ++i) {
			const auto& edge = edges[i];
			const auto& other_edge = other.edges[i];
			if (edge.innovation_id == other_edge.innovation_id) {
				result.edges.emplace(edge.crossover(other_edge));
				continue;
			}
			result.edges.emplace(edge);
		}*/

	}

	std::vector<float> NeatNetwork::evaluate(const std::vector<float>& input_data)
	{
		assert(input_data.size() >= input_neurons.size());
		std::map<int, Neuron> neuron_map;
		for (unsigned i = 0; i < input_neurons.size(); ++i) {
			input_neurons[i]->current_input = input_data[i];
		}
		
		SharedNeuronSet not_visited(input_neurons.begin(), input_neurons.end());
		while (!not_visited.empty()) {
			SharedNeuronSet to_visit_next;
			for (auto& neuron: not_visited) {
				evaluate_neuron(*neuron);
				for (const auto& edge : edges) {
					if (edge.id.from_id == neuron->id) {
						auto next_neuron = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
							return neuron->id == edge.id.to_id;
						});
						auto is_leading_to_output = false;
						for (const auto& output_neuron : output_neurons) {
							if (output_neuron->id == edge.id.to_id) {
								is_leading_to_output = true;
								break;
							}
						}
						if (!is_leading_to_output) {
							to_visit_next.insert(*next_neuron);
						}
					}
				}
			}
			not_visited = to_visit_next;
		}
		std::vector<float> results(output_neurons.size());
		for (unsigned i = 0; i < output_neurons.size(); ++i) {
			results[i] = output_neurons[i]->evaluate();
		}
		reset_intermediate_calculations();

		return results;
	}

	EdgeId::EdgeId(int from_id, int to_id)
		: from_id(from_id)
		, to_id(to_id)
	{
	}

	bool EdgeId::operator<(const EdgeId& other) const
	{
		if (from_id == other.from_id) {
			return to_id < other.to_id;
		}
		return from_id < other.from_id;
	}

	bool EdgeId::operator==(const EdgeId& other) const
	{
		return from_id == other.from_id && to_id == other.to_id;
	}

	std::size_t EdgeId::operator()() const
	{
		return from_id ^ to_id;
	}

} // namespace neural_network
