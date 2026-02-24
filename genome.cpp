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
		enum class MutationChoice { ADD_NEURON, ADD_EDGE, CHANGE_WEIGHT, TOGGLE_EDGE };
		constexpr std::array mutation_choices = {
			MutationChoice::ADD_NEURON,
			MutationChoice::ADD_EDGE,
			MutationChoice::CHANGE_WEIGHT,
			MutationChoice::TOGGLE_EDGE
		};
		const auto choice = random_element(mutation_choices);

		switch (choice) {
		case MutationChoice::ADD_NEURON:
			mutate_add_neuron();
			break;
		case MutationChoice::ADD_EDGE:
			mutate_add_edge();
			break;
		case MutationChoice::CHANGE_WEIGHT:
			mutate_change_weight();
			break;
		case MutationChoice::TOGGLE_EDGE:
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
