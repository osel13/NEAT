#include <vector>

#include "individual.h"
#include "population.h"

namespace neat {
	Population::Population(int size, float speciation_threshold)
		: size(size)
		, speciation_threshold(speciation_threshold)
		, generation(0)
		, average_fitness(0)
		, best_fitness(0)
		, best_fitness_individual_id(0)
		, last_innovation_id(0)
		, generations_without_improvement(0)
	{
		individuals.reserve(size);
		for (int i = 0; i < size; i++) {
			individuals.emplace_back(Genome(1, 1));
		}
	}

	void Population::print(std::ostream& stream) const
	{
		for (const auto& individual : individuals) {
			stream << individual.get_fitness() << std::endl;
		}
	}
	void Population::crossover()
	{
		// TODO: implement
		// for now we just use two individuals and cross them over
		individuals[0] = individuals[0].crossover(individuals[1]);
	}
} // namespace neat