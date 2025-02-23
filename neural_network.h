#pragma once

#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <cassert>
#include <ostream>
#include <istream>
#include <memory>
#include <typeindex> 
#include "utils.h"
#include "activation_function.hpp"
#include "neuron.h"
#include "edge.h"

namespace neural_network {

	class NeatNetwork {
		static int next_id;
		using SharedNeuronReference = std::shared_ptr<Neuron>;
		using SharedNeuronSet = std::set<SharedNeuronReference>;
		using SharedNeurons = std::vector<SharedNeuronReference>;
		using Neurons = std::vector<SharedNeuronReference>; // this could possibly be a (unordered) set later
		using Edges = std::unordered_set<Edge, EdgeHasher>;

		int id;
		int next_edge_innovation_number;
		int next_neuron_innovation_number;
		float fitness;
		Neurons neurons;
		Edges edges;
		SharedNeurons input_neurons;
		SharedNeurons output_neurons;

		void mutate_add_neuron();
		void reset_intermediate_calculations();
		void add_neuron(const SharedNeuronReference& new_neuron, const NeatNetwork& old_network, const SharedNeuronReference& old_neuron);
		void evaluate_neuron(const Neuron& neuron);
		void evaluate_layer(const Neurons& layer);
		void create_special_neuron(SharedNeurons& special_neuron_container, unsigned int count = 1);
		void crossover_neurons(NeatNetwork& result, const NeatNetwork& other) const;
		void crossover_edges(NeatNetwork& result, const NeatNetwork& other) const;
	public:
		NeatNetwork();
		NeatNetwork(unsigned input_neurons, unsigned output_neurons);
		NeatNetwork crossover(const NeatNetwork& other) const;
		std::vector<float> evaluate(const std::vector<float>& input_data);
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
