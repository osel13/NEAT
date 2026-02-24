#include <set>
#include <unordered_map>
#include "neural_network.h"
#include "utils.h"

namespace neural_network {
	int NeatNetwork::next_id = 0;

	namespace {
		constexpr float WEIGHT_MUTATION_MIN_DELTA = -0.25f;
		constexpr float WEIGHT_MUTATION_MAX_DELTA = 0.25f;
	}

	Neuron* NeatNetwork::get_neuron_by_id(int id)
	{
		const auto neuron_match = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
			return neuron->id == id;
		});
		if (neuron_match == neurons.end()) {
			return nullptr;
		}
		return neuron_match->get();
	}

	std::vector<Edge*> NeatNetwork::get_edges_from_neuron(const Neuron& neuron)
	{
		auto result = std::vector<Edge*>();
		for (const auto& edge : edges) {
			if (edge->from() == neuron.id) {
				result.push_back(edge.get());
			}
		}
		return result;
	}

	bool NeatNetwork::creating_edge_would_create_cycle(const Edge& edge) const
	{
		const auto candidate_from_id = edge.from();
		const auto candidate_to_id = edge.to();

		if (candidate_from_id == candidate_to_id) {
			return true;
		}

		auto visited_ids = std::set<int>();
		auto pending_ids = std::set<int>();
		pending_ids.insert(candidate_to_id);

		while (!pending_ids.empty()) {
			const auto current_id = *pending_ids.begin();
			if (current_id == candidate_from_id) {
				return true;
			}
			pending_ids.erase(pending_ids.begin());
			visited_ids.insert(current_id);

			for (const auto& existing_edge : edges) {
				const auto existing_from_id = existing_edge->from();
				const auto existing_to_id = existing_edge->to();
				const auto is_leading_from_current = existing_from_id == current_id;
				const auto is_same_as_candidate =
					existing_from_id == candidate_from_id && existing_to_id == candidate_to_id;
				const auto has_not_been_visited = visited_ids.find(existing_to_id) == visited_ids.end();

				if ((is_leading_from_current || is_same_as_candidate) && has_not_been_visited) {
					pending_ids.insert(existing_to_id);
				}
			}
		}
		return false;
	}

	Edge* NeatNetwork::get_edge_by_id(int from_id, int to_id)
	{
		const auto edge_id = EdgeId(from_id, to_id);
		const auto edge_match = std::find_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& edge) {
			return edge->id == edge_id;
		});
		if (edge_match == edges.end()) {
			return nullptr;
		}
		return edge_match->get();
	}

	Edge* NeatNetwork::get_edge_by_id(const Neuron& from, const Neuron& to)
	{
		return get_edge_by_id(from.id, to.id);
	}

	Edge* NeatNetwork::get_edge_by_id(const EdgeId& edge_id)
	{
		return get_edge_by_id(edge_id.from(), edge_id.to());
	}

	void NeatNetwork::mutate_add_neuron()
	{
		if (edges.empty()) {
			return;
		}

		auto& selected_edge_link = random_element(edges);
		auto& selected_edge = *selected_edge_link;
		selected_edge.set_disabled();

		auto new_neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
		neurons.emplace_back(new_neuron);

		auto from_edge = std::make_unique<Edge>(selected_edge.from(), new_neuron->id, next_edge_innovation_number++);
		from_edge->weight = 1.0f;
		edges.emplace(std::move(from_edge));

		auto to_edge = std::make_unique<Edge>(new_neuron->id, selected_edge.to(), next_edge_innovation_number++);
		to_edge->weight = selected_edge.weight;
		edges.emplace(std::move(to_edge));
	}

	void NeatNetwork::mutate_change_weight()
	{
		if (edges.empty()) {
			return;
		}
		auto& selected_edge_link = random_element(edges);
		auto& selected_edge = *selected_edge_link;
		selected_edge.weight += random_from_range(WEIGHT_MUTATION_MIN_DELTA, WEIGHT_MUTATION_MAX_DELTA);
	}

	void NeatNetwork::mutate_toggle_edge()
	{
		if (edges.empty()) {
			return;
		}
		auto& selected_edge_link = random_element(edges);
		auto& selected_edge = *selected_edge_link;
		selected_edge.set_disabled(!selected_edge.is_disabled);
	}

	void NeatNetwork::add_neuron(const SharedNeuronReference& new_neuron, const NeatNetwork& old_network, const SharedNeuronReference& old_neuron) {
		const auto found_input_neuron =
			std::find(old_network.input_neurons.begin(), old_network.input_neurons.end(), old_neuron);
		const auto is_input_neuron = found_input_neuron != old_network.input_neurons.end();
		if (is_input_neuron) {
			input_neurons.emplace_back(new_neuron);
		}

		const auto found_output_neuron =
			std::find(old_network.output_neurons.begin(), old_network.output_neurons.end(), old_neuron);
		const auto is_output_neuron = found_output_neuron != old_network.output_neurons.end();
		if (is_output_neuron) {
			output_neurons.emplace_back(new_neuron);
		}

		neurons.emplace_back(new_neuron);
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

	NeatNetwork::Neurons::const_iterator NeatNetwork::find_neuron_by_id(int neuron_id) const
	{
		return std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& candidate) {
			return candidate->id == neuron_id;
		});
	}

	void NeatNetwork::collect_next_neurons(const SharedNeuronReference& source_neuron, SharedNeuronSet& next_neurons) const
	{
		for (const auto& edge : edges) {
			if (edge->from() != source_neuron->id) {
				continue;
			}

			const auto target_id = edge->to();
			if (is_output_neuron_id(target_id)) {
				continue;
			}

			const auto next_neuron = find_neuron_by_id(target_id);
			next_neurons.insert(*next_neuron);
		}
	}

	void NeatNetwork::add_edge()
	{
		if (neurons.empty()) {
			return;
		}
		const auto& from_neuron = random_element(neurons);
		const auto& to_neuron = random_element(neurons);
		auto* existing_edge = get_edge_by_id(*from_neuron, *to_neuron);
		if (existing_edge != nullptr) {
			existing_edge->set_disabled(false);
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
		const auto outgoing_edge = std::find_if(edges.begin(), edges.end(), [&](const std::unique_ptr<Edge>& edge) {
			return edge->from() == neuron.id;
		});
		if (outgoing_edge == edges.end()) {
			return;
		}

		const auto* edge = outgoing_edge->get();
		const auto target_neuron = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& candidate) {
			return edge->to() == candidate->id;
		});
		if (target_neuron == neurons.end()) {
			return;
		}

		(*target_neuron)->current_input += edge->evaluate(neuron.evaluate());
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
			auto matching_edge = other_by_innovation.find(edge->innovation_id);
			if (matching_edge != other_by_innovation.end()) {
				result.edges.emplace(std::make_unique<Edge>(edge->crossover(*matching_edge->second)));
				continue;
			}
			result.edges.emplace(std::make_unique<Edge>(*edge));
		}
	}

	std::vector<float> NeatNetwork::evaluate(const std::vector<float>& input_data)
	{
		assert(input_data.size() >= input_neurons.size());
		for (unsigned i = 0; i < input_neurons.size(); ++i) {
			input_neurons[i]->current_input = input_data[i];
		}

		SharedNeuronSet current_neurons(input_neurons.begin(), input_neurons.end());
		while (!current_neurons.empty()) {
			SharedNeuronSet next_neurons;
			for (const auto& neuron : current_neurons) {
				evaluate_neuron(*neuron);
				collect_next_neurons(neuron, next_neurons);
			}
			current_neurons = std::move(next_neurons);
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
			if (edge->from() == from_id && edge->to() == to_id) {
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
