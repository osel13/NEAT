#pragma once

#include <vector>
#include "neural_network.h"

namespace neat {
	class Genome {
		int id;
		static int global_innovation_number;
		std::shared_ptr<neural_network::NeatNetwork> network;
	public:
		Genome(unsigned input_neurons = 1, unsigned output_neurons = 1);
		Genome(std::shared_ptr<neural_network::NeatNetwork>&& network);
		Genome crossover(const Genome& other) const;
		void mutate();
		void mutate_add_neuron();
		void mutate_add_edge();
		void mutate_change_weight();
		void mutate_toggle_edge();
	};

} // namespace neat