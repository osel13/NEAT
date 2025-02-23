#include "neural_network.h"
#include "utils.h"

namespace neural_network {

	void Neuron::reset()
	{
		current_input = 0;
	}

	Neuron::Neuron(unsigned innovation_id) 
		: id(innovation_id)
		, bias(0)
		, activation_function()
		, current_input(0)
	{
	}

	float Neuron::evaluate() const 
	{
		return activation_function.evaluate(current_input + bias);
	}

	Neuron Neuron::crossover(const Neuron& other) const 
	{
		assert(id == other.id);
		auto result = Neuron(id);
		const auto threshold = 0.5f;
		result.bias = random_thresholded_pick(threshold, bias, other.bias);
		result.activation_function = random_thresholded_pick(threshold, activation_function, other.activation_function);
		return result;
	}
	std::ostream& Neuron::operator<<(std::ostream& stream) const
	{
		const auto separator = ' ';
		return stream << id << separator << bias << separator;
	}
	std::istream& Neuron::operator>>(std::istream& stream)
	{
		return stream >> id >> std::ws >> bias;
	}
	
	Edge::Edge(unsigned from_id, unsigned to_id, unsigned innovation_id)
		: from_id(from_id)
		, to_id(to_id)
		, weight(random_from_range(-1.f, 1.f))
		, is_disabled(false)
		, id(innovation_id)
	{
	}

	void Edge::set_disabled(const bool value)
	{
		is_disabled = value;
	}

	float Edge::evaluate(const float input) const
	{
		if (is_disabled) {
			return 0;
		}
		return input * weight;
	}

	Edge Edge::crossover(const Edge& other) const
	{
		assert(id == other.id);
		auto result = Edge(*this);
		const auto threshold = 0.5;
		result.weight = random_thresholded_pick(threshold, weight, other.weight);
		result.is_disabled = is_disabled || other.is_disabled;
		return result;
	}
	
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

	NeatNetwork NeatNetwork::crossover(const NeatNetwork& other) const
	{
		// We assume that this network is dominant over the other
		// Take the dominant neurons and crossover them with recessive neuron counterparts, if they exist
		assert(id != other.id);
		NeatNetwork result;
		const auto threshold = random_from_range(0.1f, 0.9f);
		for (unsigned i = 0; i < neurons.size(); ++i) {
			const auto& neuron = neurons[i];
			const auto& other_neuron = other.neurons[i];
			if (neuron->id == other_neuron->id) {
				result.neurons.emplace_back(std::make_shared<Neuron>(neuron->crossover(*other_neuron)));
				continue;
			}
			result.neurons.emplace_back(neuron);
		}
		// Crossover edges
		for (unsigned i = 0; i < std::max(edges.size(), other.edges.size()); ++i) {
			const auto& edge = edges[i];
			const auto& other_edge = other.edges[i];
			if (edge.id == other_edge.id) {
				result.edges.emplace_back(edge.crossover(other_edge));
				continue;
			}
			result.edges.emplace_back(edge);
		}
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

	NeatNetwork::NeatNetwork(unsigned input_neurons, unsigned output_neurons)
		: NeatNetwork()
	{
		neurons.reserve(input_neurons + output_neurons);
		auto input_layer = Neurons();
		auto output_layer = Neurons();
		input_layer.reserve(input_neurons);
		output_layer.reserve(output_neurons);
		for (unsigned i = 0; i < input_neurons; i++) {
			auto neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
			input_layer.emplace_back(neuron);
			neurons.emplace_back(neuron);
		}
		for (unsigned i = 0; i < output_neurons; i++) {
			auto neuron = std::make_shared<Neuron>(next_neuron_innovation_number++);
			output_layer.emplace_back(neuron);
			neurons.emplace_back(neuron);
		}
		for (const auto& input_neuron : input_layer) {
			for (const auto& output_neuron : output_layer) {
				edges.emplace_back(Edge(input_neuron->id, output_neuron->id, next_edge_innovation_number++));
			}
		}
		layers.emplace_back(input_layer);
		layers.emplace_back(output_layer);
	}

	void NeatNetwork::evaluate_layer(const Neurons& layer)
	{
		for (const auto& neuron : layer) {
			auto edge_it = std::find_if(edges.begin(), edges.end(), [&](const Edge& edge) {
				return edge.from_id == neuron->id;
			});
			auto neuron_it = std::find_if(neurons.begin(), neurons.end(), [&](const SharedNeuronReference& neuron) {
				return edge_it->to_id == neuron->id;
			});
			(*neuron_it)->current_input += edge_it->evaluate(neuron->evaluate());
		}
	}

	std::vector<float> NeatNetwork::evaluate(const NeuronInputs& input_data)
	{
		auto& first_layer = layers.front();
		assert(input_data.size() >= first_layer.size());
		for (unsigned i = 0; i < first_layer.size(); ++i) {
			first_layer[i]->current_input = input_data[i];
		}
		for (auto it = layers.begin(); it + 1 != layers.end(); ++it) {
			evaluate_layer(*it);
		}
		const auto& last_layer = layers.back();
		std::vector<float> results(last_layer.size());
		for (unsigned i = 0; i < last_layer.size(); ++i) {
			const auto neuron = last_layer[i];
			results[i] = neuron->evaluate();
		}
		reset_intermediate_calculations();

		return results;
	}
}