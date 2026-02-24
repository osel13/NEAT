#include <set>
#include <map>
#include <unordered_map>
#include "neural_network.h"
#include "utils.h"

namespace neural_network {
	int NeatNetwork::next_id = 0;

	Neuron* NeatNetwork::get_neuron_by_id(int id)
	{
		auto neuron_it = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
			return neuron->id == id;
		});
		if (neuron_it == neurons.end()) {
			return nullptr;
		}
		return neuron_it->get();
	}

	std::vector<Edge*> NeatNetwork::get_edges_from_neuron(const Neuron& neuron)
	{
		auto result = std::vector<Edge*>();
		for (const auto& edge : edges) {
			if (edge->id.from_id == neuron.id) {
				result.push_back(edge.get());
			}
		}
		return result;
	}

	bool NeatNetwork::creating_edge_would_create_cycle(const Edge& edge) const
	{
		if (edge.id.from_id == edge.id.to_id) {
			return true;
		}
		auto visited = std::set<int>();
		auto to_visit = std::set<int>();
		to_visit.insert(edge.id.to_id);
		while (!to_visit.empty()) {
			auto current_id = *to_visit.begin();
			if (current_id == edge.id.from_id) {
				return true;
			}
			to_visit.erase(to_visit.begin());
			visited.insert(current_id);
			for (const auto& existing_edge : edges) {
				auto edge_id = existing_edge->id;
				if (edge_id.from_id == current_id && visited.find(edge_id.to_id) == visited.end()) {
					to_visit.insert(edge_id.to_id);
				}
				if (edge_id.from_id == edge.id.from_id && edge_id.to_id == edge.id.to_id) {
					if (visited.find(edge_id.to_id) == visited.end()) {
						to_visit.insert(edge_id.to_id);
					}
				}
			}
		}
		return false;
	}

	Edge* NeatNetwork::get_edge_by_id(int from_id, int to_id)
	{
		const auto edge_id = EdgeId(from_id, to_id);
		auto edge_it = std::find_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& edge) {
			return edge->id == edge_id;
		});
		if (edge_it == edges.end()) {
			return nullptr;
		}
		return edge_it->get();
	}

	Edge* NeatNetwork::get_edge_by_id(const Neuron& from, const Neuron& to)
	{
		return get_edge_by_id(from.id, to.id);
	}

	Edge* NeatNetwork::get_edge_by_id(const EdgeId& edge_id)
	{
		return get_edge_by_id(edge_id.from_id, edge_id.to_id);
	}

	void NeatNetwork::mutate_add_neuron()
	{
		if (edges.empty()) {
			return;
		}
		auto edge_it = random_from_range(edges.begin(), edges.end());
		auto& edge = **edge_it;
		edge.set_disabled();

		auto new_neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
		neurons.emplace_back(new_neuron);

		auto from_edge = std::make_unique<Edge>(edge.id.from_id, new_neuron->id, next_edge_innovation_number++);
		from_edge->weight = 1.0f;
		edges.emplace(std::move(from_edge));

		auto to_edge = std::make_unique<Edge>(new_neuron->id, edge.id.to_id, next_edge_innovation_number++);
		to_edge->weight = edge.weight;
		edges.emplace(std::move(to_edge));
	}

	void NeatNetwork::mutate_change_weight()
	{
		if (edges.empty()) {
			return;
		}
		auto edge_it = random_from_range(edges.begin(), edges.end());
		auto& edge = **edge_it;
		edge.weight += random_from_range(-0.25f, 0.25f);
	}

	void NeatNetwork::mutate_toggle_edge()
	{
		if (edges.empty()) {
			return;
		}
		auto edge_it = random_from_range(edges.begin(), edges.end());
		auto& edge = **edge_it;
		edge.set_disabled(!edge.is_disabled);
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

	void NeatNetwork::add_edge()
	{
		if (neurons.empty()) {
			return;
		}
		auto from_neuron_it = random_from_range(neurons.begin(), neurons.end());
		auto to_neuron_it = random_from_range(neurons.begin(), neurons.end());
		auto from_neuron = *from_neuron_it;
		auto to_neuron = *to_neuron_it;
		auto edge = get_edge_by_id(*from_neuron, *to_neuron);
		if (edge != nullptr) {
			edge->set_disabled(false);
			return;
		}
		auto candidate_edge = Edge(from_neuron->id, to_neuron->id, -1);
		if (creating_edge_would_create_cycle(candidate_edge)) {
			return;
		}
		auto new_edge = std::make_unique<Edge>(from_neuron->id, to_neuron->id, next_edge_innovation_number++);
		edges.insert(std::move(new_edge));
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
				edges.emplace(std::make_unique<Edge>(input_neuron->id, output_neuron->id, next_edge_innovation_number++));
			}
		}
	}

	void NeatNetwork::evaluate_neuron(const Neuron& neuron) {
		auto edge_it = std::find_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& edge) {
			return edge->id.from_id == neuron.id;
		});
		if (edge_it == edges.end()) {
			return;
		}
		auto edge = edge_it->get();
		auto neuron_it = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
			return edge->id.to_id == neuron->id;
		});
		if (neuron_it == neurons.end()) {
			return;
		}
		(*neuron_it)->current_input += edge->evaluate(neuron.evaluate());
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
			if (i < other.neurons.size() && neuron->id == other.neurons[i]->id) {
				const auto& other_neuron = other.neurons[i];
				auto new_neuron = std::make_shared<Neuron>(neuron->crossover(*other_neuron));
				result.add_neuron(new_neuron, *this, neuron);
				continue;
			}
			result.add_neuron(std::make_shared<Neuron>(*neuron), *this, neuron);
		}
	}

	void NeatNetwork::crossover_edges(NeatNetwork& result, const NeatNetwork& other) const
	{
		auto other_by_innovation = std::unordered_map<int, const Edge*>();
		for (const auto& other_edge : other.edges) {
			other_by_innovation.emplace(other_edge->innovation_id, other_edge.get());
		}

		for (const auto& edge : edges) {
			auto other_edge_it = other_by_innovation.find(edge->innovation_id);
			if (other_edge_it != other_by_innovation.end()) {
				result.edges.emplace(std::make_unique<Edge>(edge->crossover(*other_edge_it->second)));
				continue;
			}
			result.edges.emplace(std::make_unique<Edge>(*edge));
		}
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
					if (edge->id.from_id == neuron->id) {
						auto next_neuron = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
							return neuron->id == edge->id.to_id;
						});
						auto is_leading_to_output = false;
						for (const auto& output_neuron : output_neurons) {
							if (output_neuron->id == edge->id.to_id) {
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

	size_t NeatNetwork::neuron_count() const
	{
		return neurons.size();
	}

	size_t NeatNetwork::edge_count() const
	{
		return edges.size();
	}

	size_t NeatNetwork::disabled_edge_count() const
	{
		size_t result = 0;
		for (const auto& edge : edges) {
			if (edge->is_disabled) {
				++result;
			}
		}
		return result;
	}

	float NeatNetwork::edge_weight_sum() const
	{
		float result = 0.0f;
		for (const auto& edge : edges) {
			result += edge->weight;
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
		for (const auto& edge : edges) {
			if (edge->id.from_id == from_id && edge->id.to_id == to_id) {
				return true;
			}
		}
		return false;
	}

	std::size_t EdgeId::operator()() const
	{
		return from_id ^ to_id;
	}

} // namespace neural_network
