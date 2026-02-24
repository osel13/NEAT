#include <algorithm>
#include "population.h"

namespace neat {
	Population::Population(int size, float speciation_threshold)
		: size(size)
		, speciation_threshold(speciation_threshold)
		, generation(0)
		, average_fitness(0)
		, best_fitness(0)
		, best_fitness_individual_id(0)
	{
		individuals.reserve(size);
		for (int i = 0; i < size; ++i) {
			individuals.emplace_back(Genome(1, 1));
		}
	}

	void Population::print(std::ostream& stream) const
	{
		for (const auto& individual : individuals) {
			stream << individual.get_fitness() << std::endl;
		}
	}

	void Population::print_topologies(size_t count, std::ostream& stream) const
	{
		if (individuals.empty() || count == 0) {
			return;
		}

		auto ranked_indices = std::vector<size_t>();
		ranked_indices.reserve(individuals.size());
		for (size_t i = 0; i < individuals.size(); ++i) {
			ranked_indices.push_back(i);
		}
		std::sort(ranked_indices.begin(), ranked_indices.end(), [&](size_t left, size_t right) {
			return individuals[left].get_fitness() > individuals[right].get_fitness();
		});

		const auto topology_count = std::min(count, ranked_indices.size());
		stream << "\nTopologies (top " << topology_count << ")\n";
		for (size_t rank = 0; rank < topology_count; ++rank) {
			const auto index = ranked_indices[rank];
			const auto fitness = individuals[index].get_fitness();
			stream << "\n#" << (rank + 1) << " fitness=" << fitness << "\n";
			stream << individuals[index].get_genome().get_network().topology_diagram();
		}
	}

	void Population::next_generation()
	{
		evaluate_fitness();
		select();
		speciate();
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

	void Population::crossover()
	{
		if (individuals.empty()) {
			return;
		}

		auto next_generation = std::vector<Individual>();
		next_generation.reserve(size);
		next_generation.emplace_back(individuals[best_fitness_individual_id]);

		while (static_cast<int>(next_generation.size()) < size) {
			const auto& selected_species = pick_species();
			const auto& parent_a = pick_parent_from_species(selected_species);
			auto* parent_b = &pick_parent_from_species(selected_species);
			if (individuals.size() > 1) {
				const auto empty_species = std::vector<size_t>();
				for (int attempt = 0; attempt < 32 && &parent_a == parent_b; ++attempt) {
					const auto should_pick_globally = selected_species.size() <= 1;
					parent_b = should_pick_globally
						? &pick_parent_from_species(empty_species)
						: &pick_parent_from_species(selected_species);
				}
			}
			if (&parent_a == parent_b) {
				next_generation.emplace_back(parent_a);
				continue;
			}
			next_generation.emplace_back(parent_a.crossover(*parent_b));
		}

		individuals = std::move(next_generation);
		best_fitness_individual_id = 0;
		species.clear();
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
		species.clear();
	}
} // namespace neat
