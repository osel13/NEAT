#include <algorithm>
#include <random>

#include "neural_network.h"
#include "utils.h"

namespace neural_network {
	int NeatNetwork::next_id = 0;

	NeatNetwork::NeatNetwork()
		: id(NeatNetwork::next_id++)
		, next_edge_innovation_number(0)
		, next_neuron_innovation_number(0)
		, fitness(0)
	{
	}

	NeatNetwork::NeatNetwork(unsigned input_neuron_count, unsigned output_neuron_count)
		: NeatNetwork()
	{
		create_special_neuron(input_neurons, input_neuron_count);
		create_special_neuron(output_neurons, output_neuron_count);
		for (const auto& input_neuron : input_neurons) {
			for (const auto& output_neuron : output_neurons) {
				insert_edge(std::make_unique<Edge>(input_neuron->id, output_neuron->id, next_edge_innovation_number++));
			}
		}
	}

	std::shared_ptr<Neuron> NeatNetwork::get_neuron_by_id(int id) const
	{
		const auto neuron_match = neurons_by_id.find(id);
		if (neuron_match == neurons_by_id.end()) {
			return nullptr;
		}
		return neuron_match->second;
	}

	std::vector<Edge*> NeatNetwork::get_edges_from_neuron(const Neuron& neuron)
	{
		const auto edges_match = outgoing_edges_by_neuron_id.find(neuron.id);
		if (edges_match == outgoing_edges_by_neuron_id.end()) {
			return {};
		}

		return edges_match->second;
	}

	Edge* NeatNetwork::get_edge_by_id(int from_id, int to_id)
	{
		return get_edge_by_id(EdgeId(from_id, to_id));
	}

	Edge* NeatNetwork::get_edge_by_id(const Neuron& from, const Neuron& to)
	{
		return get_edge_by_id(from.id, to.id);
	}

	Edge* NeatNetwork::get_edge_by_id(const EdgeId& edge_id)
	{
		const auto edge_match = edges_by_id.find(edge_id);
		if (edge_match == edges_by_id.end()) {
			return nullptr;
		}
		return edge_match->second.get();
	}

	const Edge* NeatNetwork::get_edge_by_innovation_id(int innovation_id) const
	{
		const auto edge_match = edges_by_innovation.find(innovation_id);
		if (edge_match == edges_by_innovation.end()) {
			return nullptr;
		}
		return edge_match->second;
	}

	void NeatNetwork::add_neuron(const std::shared_ptr<Neuron>& new_neuron)
	{
		neurons_by_id.insert_or_assign(new_neuron->id, new_neuron);
		next_neuron_innovation_number = std::max(next_neuron_innovation_number, new_neuron->id + 1);
	}

	void NeatNetwork::add_neuron(const std::shared_ptr<Neuron>& new_neuron, const NeatNetwork& old_network, const std::shared_ptr<Neuron>& old_neuron)
	{
		const auto is_input_neuron =
			std::find(old_network.input_neurons.begin(), old_network.input_neurons.end(), old_neuron) != old_network.input_neurons.end();
		if (is_input_neuron) {
			input_neurons.emplace_back(new_neuron);
		}

		const auto is_output_neuron =
			std::find(old_network.output_neurons.begin(), old_network.output_neurons.end(), old_neuron) != old_network.output_neurons.end();
		if (is_output_neuron) {
			output_neurons.emplace_back(new_neuron);
		}

		add_neuron(new_neuron);
	}

	bool NeatNetwork::is_output_neuron_id(int neuron_id) const
	{
		for (const auto& output_neuron : output_neurons) {
			if (output_neuron->id == neuron_id) {
				return true;
			}
		}
		return false;
	}

	void NeatNetwork::create_special_neuron(std::vector<std::shared_ptr<Neuron>>& special_neuron_container, unsigned int count)
	{
		for (unsigned i = 0; i < count; ++i) {
			auto neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
			special_neuron_container.emplace_back(neuron);
			neurons_by_id.emplace(neuron->id, neuron);
		}
	}

	std::shared_ptr<Neuron> NeatNetwork::pick_random_neuron() const
	{
		if (neurons_by_id.empty() || next_neuron_innovation_number <= 0) {
			return nullptr;
		}

		std::uniform_int_distribution<int> distribution(0, next_neuron_innovation_number - 1);
		for (int attempt = 0; attempt < 64; ++attempt) {
			const auto id = distribution(default_generator);
			auto neuron_match = neurons_by_id.find(id);
			if (neuron_match != neurons_by_id.end()) {
				return neuron_match->second;
			}
		}
		return nullptr;
	}

	Edge* NeatNetwork::pick_random_edge() const
	{
		if (edges_by_innovation.empty() || next_edge_innovation_number <= 0) {
			return nullptr;
		}

		std::uniform_int_distribution<int> distribution(0, next_edge_innovation_number - 1);
		for (int attempt = 0; attempt < 64; ++attempt) {
			const auto innovation_id = distribution(default_generator);
			auto edge_match = edges_by_innovation.find(innovation_id);
			if (edge_match != edges_by_innovation.end()) {
				return edge_match->second;
			}
		}
		return nullptr;
	}

	void NeatNetwork::insert_edge(std::unique_ptr<Edge> edge)
	{
		auto edge_id = edge->id;
		Edge* edge_ptr = edge.get();
		auto insertion = edges_by_id.emplace(edge_id, std::move(edge));
		if (!insertion.second) {
			return;
		}

		edges_by_innovation.insert_or_assign(edge_ptr->innovation_id, edge_ptr);
		outgoing_edges_by_neuron_id[edge_ptr->from()].emplace_back(edge_ptr);
		next_edge_innovation_number = std::max(next_edge_innovation_number, edge_ptr->innovation_id + 1);
	}

	void NeatNetwork::rebuild_outgoing_edges()
	{
		outgoing_edges_by_neuron_id.clear();
		for (const auto& edge_entry : edges_by_id) {
			Edge* edge = edge_entry.second.get();
			outgoing_edges_by_neuron_id[edge->from()].emplace_back(edge);
		}
	}

	void NeatNetwork::update_next_edge_innovation_number()
	{
		next_edge_innovation_number = 0;
		for (const auto& edge_entry : edges_by_id) {
			next_edge_innovation_number = std::max(next_edge_innovation_number, edge_entry.second->innovation_id + 1);
		}
	}

} // namespace neural_network
