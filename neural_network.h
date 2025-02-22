#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <ostream>
#include "utils.h"

namespace neural_network {
	class ActivationFunction {

	public:
		float evaluate(const float input) const {
			return input;
		};
	};

	class Neuron {
		int id;
		float bias;
		ActivationFunction activation_function;
	public:
		float evaluate(const float input) const {
			return activation_function.evaluate(input + bias);
		}

		Neuron crossover(const Neuron& other) {
			assert(id == other.id);
			auto result = Neuron();
			const auto threshold = 0.5f;
			result.id = id;
			result.bias = random_thresholded_pick(threshold, bias, other.bias);
			result.activation_function = random_thresholded_pick(threshold, activation_function, other.activation_function);
			return result;
		}

		std::ostream& operator<<(std::ostream& stream) const {
			const auto separator = ' ';
			stream << id << separator << bias << separator;
		}

		std::istream& operator>>(std::istream& stream) {
			stream >> id >> std::ws >> bias;
		}
	};

	class Edge {
		int id;
		int from_id;
		int to_id;
		float weight;
		bool is_disabled;
	public:
		void set_disabled(const bool value = true) {
			is_disabled = value;
		}

		float evaluate (const float input) const {
			if (is_disabled) {
				return 0;
			}
			return input * weight;
		}

		Edge crossover(const Edge& other) {
			assert(id == other.id);
			auto result = Edge();
			const auto threshold = 0.5;
			result.id = id;
			result.from_id = from_id;
			result.to_id = to_id;
			result.weight = random_thresholded_pick(threshold, weight, other.weight);
			result.is_disabled = is_disabled || other.is_disabled;
			return result;
		}
	};

	class NeatNetwork {
		using Neurons = std::vector<Neuron>;
		using NeuronInputs = std::vector<float>;

		int id;
		float fitness;
		Neurons neurons;
		std::vector<Edge> edges;
		std::vector<Neurons> layers;

		void mutate_add_neuron() {
			if (edges.empty()) {
				// no edges to split
				return;
			}
			//auto& edge = random_select(edges);
		}

		NeuronInputs evaluate_layer(const Neurons& layer, const NeuronInputs& input_data) const {
			NeuronInputs neuron_values(layer.size());
			for (int i = 0; i < layer.size(); i++) {
				const auto& neuron = layer[i];
				const auto value = neuron.evaluate(input_data[i]);
				neuron_values[i] += value;
			}
			return neuron_values;
		}

		float evaluate(NeuronInputs input_data) {
			const auto max_layer_size = 10;
			NeuronInputs neuron_values(max_layer_size);
			std::move(input_data.begin(), input_data.end(), neuron_values.begin());
			for (const auto& layer : layers) {
				neuron_values = evaluate_layer(layer, input_data);
			}
		}
	};


	template <typename InputOutputType, typename WeightType = float>
	class Network {
		unsigned int input_size;
		unsigned int output_size;
		std::vector<std::vector<WeightType>> weights;
	public:
		Network(unsigned int input_size = 2, unsigned int output_size = 1);

		std::vector<InputOutputType> evaluate(std::array<InputOutputType> input_data);

		/*
		// NEAT notes:
		// - never reduce gene count
		// - global innovation number, once set it never changes
		// - same mutation has to have same innovation number, to prevent exploding innovation number
		// - same innovation number => matching gene
		// - disjoint genes = not matching and innovation < max_common_innovation, excess genes = not matching innovation > max_common_innovation
		// - start diversifying fast, slow down later
		mutation_add_node() {
			pick an existing edge
			disable edge (A-B)
			add edge to new node (A-NEW), weight 1
			add edge from new node (NEW-B), weight of disabled edge
		}

		mutation_add_edge() {
			pick two nodes not having an edge
			add edge between them
		}

		mutation_change_weight() {
			pick an edge
			change weight
		}

		crossover() {
			align matching genes
			choose randomly from disjoint genes
			choose randomly from excess genes
			if edge is disabled in either parent, disable in child
		}
		*/
	};

	template <typename InputOutputType, typename WeightType>
	Network<InputOutputType, WeightType>::Network(unsigned int input_size, unsigned int output_size)
		: input_size(input_size),
		output_size(output_size),
		weights()
	{
	}

	template <typename InputOutputType, typename WeightType, unsigned int size>
	std::array<InputOutputType, size> Network<InputOutputType, WeightType>::evaluate(std::array<InputOutputType, size> input_data)
	{
		std::array<InputOutputType> output_data;

		return output_data;
	}
}