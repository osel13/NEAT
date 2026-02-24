#include <queue>
#include <unordered_set>

#include "neural_network.h"
#include "utils.h"

namespace neural_network {
	namespace {
		constexpr float WEIGHT_MUTATION_MIN_DELTA = -0.25f;
		constexpr float WEIGHT_MUTATION_MAX_DELTA = 0.25f;
	}

	bool NeatNetwork::creating_edge_would_create_cycle(const Edge& edge) const
	{
		const auto candidate_from_id = edge.from();
		const auto candidate_to_id = edge.to();
		if (candidate_from_id == candidate_to_id) {
			return true;
		}

		auto pending_ids = std::queue<int>();
		auto visited_ids = std::unordered_set<int>();
		pending_ids.push(candidate_to_id);

		while (!pending_ids.empty()) {
			const auto current_id = pending_ids.front();
			pending_ids.pop();

			if (current_id == candidate_from_id) {
				return true;
			}
			if (!visited_ids.insert(current_id).second) {
				continue;
			}

			const auto outgoing_match = outgoing_edges_by_neuron_id.find(current_id);
			if (outgoing_match == outgoing_edges_by_neuron_id.end()) {
				continue;
			}

			for (const auto* outgoing_edge : outgoing_match->second) {
				if (visited_ids.find(outgoing_edge->to()) != visited_ids.end()) {
					continue;
				}
				pending_ids.push(outgoing_edge->to());
			}
		}

		return false;
	}

	void NeatNetwork::mutate_add_neuron()
	{
		auto* selected_edge = pick_random_edge();
		if (selected_edge == nullptr) {
			return;
		}

		selected_edge->set_disabled();

		auto new_neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
		add_neuron(new_neuron);

		auto from_edge = std::make_unique<Edge>(selected_edge->from(), new_neuron->id, next_edge_innovation_number++);
		from_edge->weight = 1.0f;
		insert_edge(std::move(from_edge));

		auto to_edge = std::make_unique<Edge>(new_neuron->id, selected_edge->to(), next_edge_innovation_number++);
		to_edge->weight = selected_edge->weight;
		insert_edge(std::move(to_edge));
	}

	void NeatNetwork::mutate_change_weight()
	{
		auto* selected_edge = pick_random_edge();
		if (selected_edge == nullptr) {
			return;
		}

		selected_edge->weight += random_from_range(WEIGHT_MUTATION_MIN_DELTA, WEIGHT_MUTATION_MAX_DELTA);
	}

	void NeatNetwork::mutate_toggle_edge()
	{
		auto* selected_edge = pick_random_edge();
		if (selected_edge == nullptr) {
			return;
		}

		selected_edge->set_disabled(!selected_edge->is_disabled);
	}

	void NeatNetwork::add_edge()
	{
		const auto from_neuron = pick_random_neuron();
		const auto to_neuron = pick_random_neuron();
		if (from_neuron == nullptr || to_neuron == nullptr) {
			return;
		}

		auto* existing_edge = get_edge_by_id(*from_neuron, *to_neuron);
		if (existing_edge != nullptr) {
			existing_edge->set_disabled(false);
			return;
		}

		auto candidate_edge = Edge(from_neuron->id, to_neuron->id, -1);
		if (creating_edge_would_create_cycle(candidate_edge)) {
			return;
		}

		insert_edge(std::make_unique<Edge>(from_neuron->id, to_neuron->id, next_edge_innovation_number++));
	}
} // namespace neural_network
