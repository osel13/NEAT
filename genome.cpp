#include "genome.h"
#include <memory>

namespace neat {

	Genome::Genome(unsigned input_neurons, unsigned output_neurons)
		: id(Genome::global_innovation_number++)
		, network(std::make_shared<neural_network::NeatNetwork>(input_neurons, output_neurons))
	{
	}

	Genome::Genome(std::shared_ptr<neural_network::NeatNetwork>&& network)
		: id(Genome::global_innovation_number++)
		, network(std::move(network))
	{
	}

	Genome Genome::crossover(const Genome& other) const
	{
		return Genome(std::make_shared<neural_network::NeatNetwork>(network->crossover(*other.network)));
	}

	void Genome::mutate()
	{
		// todo select random mutation depending on probability
	}

	int Genome::global_innovation_number = 0;
} // namespace neat
