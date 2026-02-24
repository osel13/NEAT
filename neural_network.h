#pragma once

#include <vector>
#include <set>
#include <memory>
#include <string>
#include <unordered_map>
#include "neuron.h"
#include "edge.h"

	namespace neural_network {

	class NeatNetwork {
	public:
		struct EdgeGene {
			int innovation_id;
			float weight;
			bool is_disabled;
		};
	private:
		static int next_id;
		using SharedNeuronReference = std::shared_ptr<Neuron>;
		using SharedNeuronSet = std::set<SharedNeuronReference>;
		using SharedNeurons = std::vector<SharedNeuronReference>;
		using NeuronById = std::unordered_map<int, SharedNeuronReference>;
		using EdgeById = std::unordered_map<EdgeId, std::unique_ptr<Edge>, EdgeHasher>;
		using OutgoingEdgesByNeuronId = std::unordered_map<int, std::vector<Edge*>>;

		int id;
		int next_edge_innovation_number;
		int next_neuron_innovation_number;
		float fitness;
		NeuronById neurons_by_id;
		EdgeById edges_by_id;
		std::unordered_map<int, Edge*> edges_by_innovation;
		OutgoingEdgesByNeuronId outgoing_edges_by_neuron_id;
		SharedNeurons input_neurons;
		SharedNeurons output_neurons;

		std::shared_ptr<Neuron> get_neuron_by_id(int id) const;
		std::vector<Edge*> get_edges_from_neuron(const Neuron& neuron);
		bool creating_edge_would_create_cycle(const Edge& edge) const;
		Edge* get_edge_by_id(int from_id, int to_id);
		Edge* get_edge_by_id(const Neuron& from, const Neuron& to);
		Edge* get_edge_by_id(const EdgeId& edge_id);
		const Edge* get_edge_by_innovation_id(int innovation_id) const;
		void reset_intermediate_calculations();
		void add_neuron(const std::shared_ptr<Neuron>& new_neuron, const NeatNetwork& old_network, const std::shared_ptr<Neuron>& old_neuron);
		void add_neuron(const std::shared_ptr<Neuron>& new_neuron);
		bool is_output_neuron_id(int neuron_id) const;
		void collect_next_neurons(const std::shared_ptr<Neuron>& source_neuron, std::set<std::shared_ptr<Neuron>>& next_neurons) const;
		void evaluate_neuron(const Neuron& neuron);
		void create_special_neuron(std::vector<std::shared_ptr<Neuron>>& special_neuron_container, unsigned int count = 1);
		void crossover_neurons(NeatNetwork& result, const NeatNetwork& other) const;
		void crossover_edges(NeatNetwork& result, const NeatNetwork& other) const;
		std::shared_ptr<Neuron> pick_random_neuron() const;
		Edge* pick_random_edge() const;
		void insert_edge(std::unique_ptr<Edge> edge);
		void rebuild_outgoing_edges();
		void update_next_edge_innovation_number();
		std::vector<EdgeGene> sorted_edge_genes() const;
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
		std::vector<EdgeGene> edge_genes() const;
		std::string topology_diagram(bool use_invisible_links_for_disabled_edges = true) const;
	};
}
