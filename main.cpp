
#include "neural_network.h"
#include <iostream>

int main(int argc, char* argv[]) {
	auto network = neural_network::NeatNetwork();
	auto output = network.evaluate({ 1 });
	return 0;
}