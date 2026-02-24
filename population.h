#pragma once

#include <vector>
#include <iostream>
#include <cstddef>

#include "individual.h"

	namespace neat {
	class Population {
		int size;
		float speciation_threshold;
		int generation;
		float average_fitness;
		float best_fitness;
		int best_fitness_individual_id;
		std::vector<Individual> individuals;
		std::vector<std::vector<size_t>> species;

		float compatibility_distance(const Individual& left, const Individual& right) const;
		const std::vector<size_t>& pick_species() const;
		const Individual& pick_parent_from_species(const std::vector<size_t>& species_indices) const;
	public:
		Population(int size, float speciation_threshold);
		void next_generation();
		void evaluate_fitness();
		void speciate();
		void crossover();
		void mutate();
		void select();
		void print(std::ostream& = std::cout) const;
		void print_topologies(size_t count, std::ostream& = std::cout) const;
	};
}
