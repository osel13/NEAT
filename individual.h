#pragma once

#include "genome.h"

namespace neat {
	class Individual {
		Genome genome;
		float fitness;
	public:
		Individual(const Genome& genome);
		Individual(Genome&& genome);
		Individual crossover(const Individual& other) const;
		void mutate();
		void evaluate_fitness();
		float get_fitness() const;
	};
}