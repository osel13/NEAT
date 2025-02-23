#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <ostream>
#include <istream>
#include <memory>
#include "utils.h"
#include "activation_function.hpp"

namespace neural_network {
	
	class Neuron {
		static int global_innovation_number;
		int id;
		float bias;
		float current_input;
		ActivationFunction activation_function;

		void reset();
		friend class NeatNetwork;
	public:
		Neuron();

		float evaluate() const;
		Neuron crossover(const Neuron& other) const;

		std::ostream& operator<<(std::ostream& stream) const;
		std::istream& operator>>(std::istream& stream);
	};

	class Edge {
		static int global_innovation_number;
		int id;
		int from_id;
		int to_id;
		float weight;
		bool is_disabled;
		friend class NeatNetwork;
	public:
		Edge(unsigned from_id, unsigned to_id);

		void set_disabled(const bool value = true);
		float evaluate(const float input) const;
		Edge crossover(const Edge& other);
	};

	class NeatNetwork {
		using SharedNeuronReference = std::shared_ptr<Neuron>;
		using Neurons = std::vector<SharedNeuronReference>;
		using NeuronInputs = std::vector<float>;
		using NeuronOutputs = std::vector<float>;

		int id;
		float fitness;
		Neurons neurons;
		std::vector<Edge> edges;
		std::vector<Neurons> layers;

		void mutate_add_neuron();
		void reset_intermediate_calculations();
		void evaluate_layer(const Neurons& layer);
	public:
		NeatNetwork(unsigned input_neurons = 1, unsigned output_neurons = 1);
		std::vector<float> evaluate(const NeuronInputs& input_data);
	};

	/*
	// NEAT notes:
	// - never reduce gene count, only disable some genes
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
}