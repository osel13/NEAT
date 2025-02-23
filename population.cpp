#include <vector>

class Genome {

};

class Species {
	int id;

};

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
	std::vector<Genome> genomes;
	std::vector<Species> species;

};