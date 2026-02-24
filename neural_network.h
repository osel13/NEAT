#pragma once

#include <array>
#include <vector>
#include <ranges>
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
		struct EdgeEqual {
			bool operator()(const std::unique_ptr<Edge>& left, const std::unique_ptr<Edge>& right) const {
				return left->id == right->id;
			}
		};
		using SharedNeuronReference = std::shared_ptr<Neuron>;
		using SharedNeuronSet = std::set<SharedNeuronReference>;
		using SharedNeurons = std::vector<SharedNeuronReference>;
		using Neurons = std::vector<SharedNeuronReference>; // this could possibly be a (unordered) set later
		using Edges = std::unordered_set<std::unique_ptr<Edge>, EdgeHasher, EdgeEqual>;

		int id;
		int next_edge_innovation_number;
		int next_neuron_innovation_number;
		float fitness;
		Neurons neurons;
		Edges edges;
		SharedNeurons input_neurons;
		SharedNeurons output_neurons;

		Neuron* get_neuron_by_id(int id);
		std::vector<Edge*> get_edges_from_neuron(const Neuron& neuron);
		bool creating_edge_would_create_cycle(const Edge& edge) const;
		Edge* get_edge_by_id(int from_id, int to_id);
		Edge* get_edge_by_id(const Neuron& from, const Neuron& to);
		Edge* get_edge_by_id(const EdgeId& edge_id);
		void reset_intermediate_calculations();
		void add_neuron(const SharedNeuronReference& new_neuron, const NeatNetwork& old_network, const SharedNeuronReference& old_neuron);
		bool is_output_neuron_id(int neuron_id) const;
		Neurons::const_iterator find_neuron_by_id(int neuron_id) const;
		void collect_next_neurons(const SharedNeuronReference& source_neuron, SharedNeuronSet& next_neurons) const;
		void evaluate_neuron(const Neuron& neuron);
		void evaluate_layer(const Neurons& layer);
		void create_special_neuron(SharedNeurons& special_neuron_container, unsigned int count = 1);
		void crossover_neurons(NeatNetwork& result, const NeatNetwork& other) const;
		void crossover_edges(NeatNetwork& result, const NeatNetwork& other) const;
	public:
		void add_edge();
		void mutate_add_neuron();
		void mutate_change_weight();
		void mutate_toggle_edge();
		NeatNetwork();
		NeatNetwork(unsigned input_neurons, unsigned output_neurons);
		NeatNetwork crossover(const NeatNetwork& other) const;
		std::vector<float> evaluate(const std::vector<float>& input_data);
		std::size_t neuron_count() const;
		std::size_t edge_count() const;
		std::size_t disabled_edge_count() const;
		float edge_weight_sum() const;
		std::vector<int> input_neuron_ids() const;
		std::vector<int> output_neuron_ids() const;
		bool has_edge(int from_id, int to_id) const;
	};
}
