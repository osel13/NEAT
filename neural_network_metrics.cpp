#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_set>

#include "neural_network.h"

namespace neural_network {
	size_t NeatNetwork::neuron_count() const
	{
		return neurons_by_id.size();
	}

	size_t NeatNetwork::edge_count() const
	{
		return edges_by_id.size();
	}

	size_t NeatNetwork::disabled_edge_count() const
	{
		size_t result = 0;
		for (const auto& edge_entry : edges_by_id) {
			if (edge_entry.second->is_disabled) {
				++result;
			}
		}
		return result;
	}

	float NeatNetwork::edge_weight_sum() const
	{
		float result = 0.0f;
		for (const auto& edge_entry : edges_by_id) {
			result += edge_entry.second->weight;
		}
		return result;
	}

	std::vector<int> NeatNetwork::input_neuron_ids() const
	{
		auto result = std::vector<int>();
		result.reserve(input_neurons.size());
		for (const auto& neuron : input_neurons) {
			result.push_back(neuron->id);
		}
		return result;
	}

	std::vector<int> NeatNetwork::output_neuron_ids() const
	{
		auto result = std::vector<int>();
		result.reserve(output_neurons.size());
		for (const auto& neuron : output_neurons) {
			result.push_back(neuron->id);
		}
		return result;
	}

	bool NeatNetwork::has_edge(int from_id, int to_id) const
	{
		return edges_by_id.find(EdgeId(from_id, to_id)) != edges_by_id.end();
	}

	std::vector<NeatNetwork::EdgeGene> NeatNetwork::sorted_edge_genes() const
	{
		auto genes = std::vector<EdgeGene>();
		genes.reserve(edges_by_id.size());
		for (const auto& edge_entry : edges_by_id) {
			genes.push_back({
				edge_entry.second->innovation_id,
				edge_entry.second->weight,
				edge_entry.second->is_disabled
			});
		}
		std::sort(genes.begin(), genes.end(), [](const EdgeGene& left, const EdgeGene& right) {
			return left.innovation_id < right.innovation_id;
		});
		return genes;
	}

	std::vector<NeatNetwork::EdgeGene> NeatNetwork::edge_genes() const
	{
		return sorted_edge_genes();
	}

	std::string NeatNetwork::topology_diagram(bool use_invisible_links_for_disabled_edges) const
	{
		auto output_ids = std::unordered_set<int>();
		for (const auto& neuron : output_neurons) {
			output_ids.insert(neuron->id);
		}

		auto input_ids = std::unordered_set<int>();
		for (const auto& neuron : input_neurons) {
			input_ids.insert(neuron->id);
		}

		auto hidden_ids = std::vector<int>();
		hidden_ids.reserve(neurons_by_id.size());
		for (const auto& neuron_entry : neurons_by_id) {
			const auto neuron_id = neuron_entry.first;
		if (input_ids.find(neuron_id) != input_ids.end() || output_ids.find(neuron_id) != output_ids.end()) {
			continue;
		}
			hidden_ids.push_back(neuron_id);
		}
		std::sort(hidden_ids.begin(), hidden_ids.end());

		auto input_ids_ordered = input_neuron_ids();
		auto output_ids_ordered = output_neuron_ids();
		auto edges = std::vector<const Edge*>();
		edges.reserve(edges_by_id.size());
		for (const auto& edge_entry : edges_by_id) {
			edges.push_back(edge_entry.second.get());
		}
		std::sort(edges.begin(), edges.end(), [](const Edge* left, const Edge* right) {
			if (left->from() != right->from()) {
				return left->from() < right->from();
			}
			return left->to() < right->to();
		});

		auto stream = std::ostringstream();
		stream << "```mermaid\n";
		stream << "graph LR\n";

		for (const auto& neuron_id : input_ids_ordered) {
			stream << "  N" << neuron_id << "([I" << neuron_id << "])\n";
		}
		for (const auto& neuron_id : hidden_ids) {
			stream << "  N" << neuron_id << "[H" << neuron_id << "]\n";
		}
		for (const auto& neuron_id : output_ids_ordered) {
			stream << "  N" << neuron_id << "([O" << neuron_id << "])\n";
		}

		if (edges.empty()) {
			stream << "  Empty[no edges]\n";
		}
		for (const auto* edge : edges) {
			auto weight_stream = std::ostringstream();
			weight_stream << std::fixed << std::setprecision(3) << edge->weight;
			const auto label = weight_stream.str();
			if (edge->is_disabled) {
				if (use_invisible_links_for_disabled_edges) {
					stream << "  N" << edge->from() << " ~~~ N" << edge->to() << "\n";
					stream << "  %% N" << edge->from() << " --> N" << edge->to() << " " << label << "\n";
				}
				else {
					stream << "  N" << edge->from() << " -- " << label << " --> N" << edge->to() << "\n";
				}
			}
			else {
				stream << "  N" << edge->from() << " == " << label << " ==> N" << edge->to() << "\n";
			}
		}
		stream << "```\n";

		return stream.str();
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
