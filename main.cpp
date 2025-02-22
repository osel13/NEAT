
#include "win_test.h"
#include "test_gpu.h"
#include "neural_network.h"
#include <iostream>

int main(int argc, char* argv[]) {
	//win_test();
	//test_gpu();
	auto network = neural_network::Network<int>();
	auto output = network.evaluate({ 1, 2, 3 });
	return 0;
}