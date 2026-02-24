// neat.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "population.h"

namespace neat {
	class EvolutionRunner {
		int population_size;
		float speciation_threshold;
		std::ostream& output_stream;
	public:
		EvolutionRunner(int population_size = 100, float speciation_threshold = 0.5f, std::ostream& output_stream = std::cout)
			: population_size(population_size)
			, speciation_threshold(speciation_threshold)
			, output_stream(output_stream)
		{
		}

		void run_evolution(int generation_count) const
		{
			Population population(population_size, speciation_threshold);
			for (int generation = 0; generation < generation_count; ++generation) {
				population.next_generation();
			}
			population.evaluate_fitness();
			// population.print(output_stream);
			population.print_topologies(3, output_stream);
		}
	};

	inline void run_evolution(int generation_count)
	{
		EvolutionRunner().run_evolution(generation_count);
	}
}
