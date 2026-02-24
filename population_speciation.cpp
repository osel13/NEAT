#include <algorithm>
#include <cmath>
#include <random>

#include "population.h"
#include "utils.h"

namespace neat {
	namespace {
		constexpr float EXCESS_COEFFICIENT = 1.0f;
		constexpr float DISJOINT_COEFFICIENT = 1.0f;
		constexpr float WEIGHT_DIFFERENCE_COEFFICIENT = 0.4f;

		size_t pick_random_index(size_t upper_bound_exclusive)
		{
			std::uniform_int_distribution<size_t> distribution(0, upper_bound_exclusive - 1);
			return distribution(default_generator);
		}
	}

	float Population::compatibility_distance(const Individual& left, const Individual& right) const
	{
		const auto left_genes = left.get_genome().get_network().edge_genes();
		const auto right_genes = right.get_genome().get_network().edge_genes();

		size_t left_index = 0;
		size_t right_index = 0;
		size_t matching_gene_count = 0;
		size_t disjoint_gene_count = 0;
		float matching_weight_difference_sum = 0.0f;

		while (left_index < left_genes.size() && right_index < right_genes.size()) {
			const auto& left_gene = left_genes[left_index];
			const auto& right_gene = right_genes[right_index];
			if (left_gene.innovation_id == right_gene.innovation_id) {
				matching_weight_difference_sum += std::fabs(left_gene.weight - right_gene.weight);
				++matching_gene_count;
				++left_index;
				++right_index;
				continue;
			}

			++disjoint_gene_count;
			if (left_gene.innovation_id < right_gene.innovation_id) {
				++left_index;
			}
			else {
				++right_index;
			}
		}

		const auto excess_gene_count = (left_genes.size() - left_index) + (right_genes.size() - right_index);
		const auto normalization = std::max<size_t>(1, std::max(left_genes.size(), right_genes.size()));
		const auto average_weight_difference =
			matching_gene_count == 0 ? 0.0f : matching_weight_difference_sum / static_cast<float>(matching_gene_count);

		return
			EXCESS_COEFFICIENT * static_cast<float>(excess_gene_count) / static_cast<float>(normalization)
			+ DISJOINT_COEFFICIENT * static_cast<float>(disjoint_gene_count) / static_cast<float>(normalization)
			+ WEIGHT_DIFFERENCE_COEFFICIENT * average_weight_difference;
	}

	void Population::speciate()
	{
		species.clear();
		for (size_t individual_index = 0; individual_index < individuals.size(); ++individual_index) {
			bool is_placed = false;
			for (auto& existing_species : species) {
				const auto representative_index = existing_species.front();
				const auto distance = compatibility_distance(individuals[individual_index], individuals[representative_index]);
				if (distance <= speciation_threshold) {
					existing_species.push_back(individual_index);
					is_placed = true;
					break;
				}
			}

			if (!is_placed) {
				species.push_back({ individual_index });
			}
		}
	}

	const std::vector<size_t>& Population::pick_species() const
	{
		if (species.empty()) {
			static const auto empty_species = std::vector<size_t>();
			return empty_species;
		}

		size_t population_sum = 0;
		for (const auto& species_group : species) {
			population_sum += species_group.size();
		}

		std::uniform_int_distribution<size_t> species_distribution(0, population_sum - 1);
		auto selected_index = species_distribution(default_generator);
		for (const auto& species_group : species) {
			if (selected_index < species_group.size()) {
				return species_group;
			}
			selected_index -= species_group.size();
		}

		return species.back();
	}

	const Individual& Population::pick_parent_from_species(const std::vector<size_t>& species_indices) const
	{
		if (species_indices.empty()) {
			const auto global_index = pick_random_index(individuals.size());
			return individuals[global_index];
		}

		const auto selected_slot = pick_random_index(species_indices.size());
		return individuals[species_indices[selected_slot]];
	}
} // namespace neat
