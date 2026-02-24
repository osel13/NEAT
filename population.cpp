#include <vector>
#include <algorithm>

#include "individual.h"
#include "population.h"
#include "utils.h"

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

	void Population::next_generation()
	{
		evaluate_fitness();
		select();
		crossover();
		mutate();
		++generation;
	}

	void Population::evaluate_fitness()
	{
		if (individuals.empty()) {
			average_fitness = 0;
			best_fitness = 0;
			best_fitness_individual_id = 0;
			return;
		}

		float fitness_sum = 0;
		best_fitness = individuals.front().get_fitness();
		best_fitness_individual_id = 0;
		for (size_t i = 0; i < individuals.size(); ++i) {
			individuals[i].evaluate_fitness();
			const auto fitness = individuals[i].get_fitness();
			fitness_sum += fitness;
			if (fitness > best_fitness) {
				best_fitness = fitness;
				best_fitness_individual_id = static_cast<int>(i);
			}
		}
		average_fitness = fitness_sum / static_cast<float>(individuals.size());
	}

	void Population::speciate()
	{
		// Placeholder: species grouping is not yet used in reproduction.
	}

	void Population::crossover()
	{
		if (individuals.empty()) {
			return;
		}
		auto next_generation = std::vector<Individual>();
		next_generation.reserve(size);
		next_generation.emplace_back(individuals[best_fitness_individual_id]);

		while (static_cast<int>(next_generation.size()) < size) {
			auto parent_a = random_from_range(individuals.begin(), individuals.end());
			auto parent_b = random_from_range(individuals.begin(), individuals.end());
			if (individuals.size() > 1) {
				while (parent_a == parent_b) {
					parent_b = random_from_range(individuals.begin(), individuals.end());
				}
			}
			next_generation.emplace_back(parent_a->crossover(*parent_b));
		}
		individuals = std::move(next_generation);
		best_fitness_individual_id = 0;
	}

	void Population::mutate()
	{
		for (size_t i = 1; i < individuals.size(); ++i) {
			individuals[i].mutate();
		}
	}

	void Population::select()
	{
		if (individuals.size() <= 2) {
			return;
		}
		std::sort(individuals.begin(), individuals.end(), [](const Individual& left, const Individual& right) {
			return left.get_fitness() > right.get_fitness();
		});
		const auto survivor_count = static_cast<size_t>(std::max(2, static_cast<int>(individuals.size() / 2)));
		individuals.erase(individuals.begin() + survivor_count, individuals.end());
		best_fitness_individual_id = 0;
	}

} // namespace neat
