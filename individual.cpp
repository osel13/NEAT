#include "individual.h"

#include <cmath>

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

	void Individual::mutate()
	{
		genome.mutate();
	}

	float Individual::get_fitness() const
	{
		return fitness;
	}

	void Individual::evaluate_fitness()
	{
		constexpr float expected_output_for_zero = 0.0f;
		constexpr float expected_output_for_one = 1.0f;
		constexpr float min_input = 0.0f;
		constexpr float max_input = 1.0f;
		constexpr float max_score = 2.0f;

		const auto output_for_zero = genome.evaluate({ min_input })[0];
		const auto output_for_one = genome.evaluate({ max_input })[0];

		const auto error_for_zero = std::fabs(output_for_zero - expected_output_for_zero);
		const auto error_for_one = std::fabs(output_for_one - expected_output_for_one);
		const auto total_error = error_for_zero + error_for_one;

		fitness = max_score - total_error;
	}
} // namespace neat
