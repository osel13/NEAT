#include "individual.h"

namespace neat {
	Individual::Individual(const Genome& genome)
		: genome(genome)
		, fitness(0)
	{
	}

	Individual::Individual(Genome&& genome)
		: genome(std::move(genome))
		, fitness(0)
	{
	}

	Individual Individual::crossover(const Individual& other) const
	{
		return genome.crossover(other.genome);
	}

	float Individual::get_fitness() const
	{
		return fitness;
	}
} // namespace neat