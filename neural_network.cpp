#include "neural_network.h"

namespace neural_network {

	void Neuron::reset()
	{
		current_input = 0;
	}

	Neuron::Neuron()
		: id(Neuron::global_innovation_number++)
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
		auto result = Neuron();
		const auto threshold = 0.5f;
		result.id = id;
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
	
	int Neuron::global_innovation_number = 0;

	Edge::Edge(unsigned from_id, unsigned to_id)
		: from_id(from_id)
		, to_id(to_id)
		, weight(1)
		, is_disabled(false)
		, id(Edge::global_innovation_number++)
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

	Edge Edge::crossover(const Edge& other)
	{
		assert(id == other.id);
		auto result = Edge(*this);
		const auto threshold = 0.5;
		result.weight = random_thresholded_pick(threshold, weight, other.weight);
		result.is_disabled = is_disabled || other.is_disabled;
		return result;
	}
	
	int Edge::global_innovation_number = 0;

	void NeatNetwork::mutate_add_neuron()
	{
		if (edges.empty()) {
			// no edges to split
			return;
		}
		// TODO: implement
		//auto& edge = random_select(edges);
	}

	void NeatNetwork::reset_intermediate_calculations()
	{
		for (auto& neuron : neurons) {
			neuron->reset();
		}
	}

	NeatNetwork::NeatNetwork(unsigned input_neurons, unsigned output_neurons)
	{
		id = 0;
		fitness = 0;
		neurons.reserve(input_neurons + output_neurons);
		auto input_layer = Neurons();
		auto output_layer = Neurons();
		input_layer.reserve(input_neurons);
		output_layer.reserve(output_neurons);
		for (unsigned i = 0; i < input_neurons; i++) {
			auto neuron = std::make_shared<Neuron>();
			input_layer.emplace_back(neuron);
			neurons.emplace_back(neuron);
		}
		for (unsigned i = 0; i < output_neurons; i++) {
			auto neuron = std::make_shared<Neuron>();
			output_layer.emplace_back(neuron);
			neurons.emplace_back(neuron);
		}
		for (const auto& input_neuron : input_layer) {
			for (const auto& output_neuron : output_layer) {
				edges.emplace_back(Edge(input_neuron->id, output_neuron->id));
			}
		}
		layers.emplace_back(input_layer);
		layers.emplace_back(output_layer);
	}

	void NeatNetwork::evaluate_layer(const Neurons& layer)
	{
		// foreach source neuron in layer
		// find all edges leading from it
		// add evaluation to destination neurons
		for (const auto& neuron : layer) {
			for (const auto& edge : edges) {
				if (edge.from_id == neuron->id) {
					for (auto& destination_neuron : neurons) {
						if (destination_neuron->id == edge.to_id) {
							destination_neuron->current_input += edge.evaluate(neuron->evaluate());
						}
					}
				}
			}
		}
		//auto edge_it = std::find_if(edges.begin(), edges.end(), [](const Edge& edge) {
		//	return edge.from_id == 0;
		//});
	}

	std::vector<float> NeatNetwork::evaluate(const NeuronInputs& input_data)
	{
		auto& first_layer = layers.front();
		//assert(input_data.size() > first_layer.size());
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