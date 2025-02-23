
#include "neural_network_tests.h"
#include "population.h"
#include <iostream>

int main(int argc, char* argv[]) {
	//neural_network::smallest_network_evaluation_test();
	neat::Population population(100, 0.5);
	population.crossover();
	population.print();
	return 0;
}