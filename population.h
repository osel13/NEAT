#pragma once

#include <vector>
#include <iostream>

#include "individual.h"

namespace neat {
	class Population {
		std::vector<std::vector<int>> innovation_ids;
		int last_innovation_id;
		int size;
		float speciation_threshold;
		int generations_without_improvement;
		int generation;
		float average_fitness;
		float best_fitness;
		int best_fitness_individual_id;
		std::vector<Individual> individuals;
		std::vector<Genome> genomes;
	public:
		Population(int size, float speciation_threshold);
		void next_generation();
		void evaluate_fitness();
		void speciate();
		void crossover();
		void mutate();
		void select();
		void evolve();
		void print(std::ostream& = std::cout) const;
	};
}