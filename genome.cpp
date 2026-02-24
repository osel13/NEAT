#include "genome.h"
#include <memory>
#include <array>

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

	std::vector<float> Genome::evaluate(const std::vector<float>& input_data) const
	{
		return network->evaluate(input_data);
	}

	void Genome::mutate()
	{
		enum class Choices { MUTATE_NEURON, MUTATE_EDGE, MUTATE_WEIGHT, MUTATE_TOGGLE_EDGE };
		constexpr std::array choices = {
			Choices::MUTATE_NEURON, 
			Choices::MUTATE_EDGE, 
			Choices::MUTATE_WEIGHT, 
			Choices::MUTATE_TOGGLE_EDGE
		};
		const auto choice = random_from_range(choices.begin(), choices.end());
		switch (*choice) {
		case Choices::MUTATE_NEURON:
			mutate_add_neuron();
			break;
		case Choices::MUTATE_EDGE:
			mutate_add_edge();
			break;
		case Choices::MUTATE_WEIGHT:
			mutate_change_weight();
			break;
		case Choices::MUTATE_TOGGLE_EDGE:
			mutate_toggle_edge();
			break;
		}
	}

	void Genome::mutate_add_edge()
	{
		network->add_edge();
	}

	void Genome::mutate_add_neuron()
	{
		network->mutate_add_neuron();
	}

	void Genome::mutate_change_weight()
	{
		network->mutate_change_weight();
	}

	void Genome::mutate_toggle_edge()
	{
		network->mutate_toggle_edge();
	}

	const neural_network::NeatNetwork& Genome::get_network() const
	{
		return *network;
	}

	int Genome::global_innovation_number = 0;
} // namespace neat
