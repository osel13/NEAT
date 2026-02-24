
#include "neat.h"
#include "tests_runner.h"

int main() {
#ifndef NDEBUG
	tests_runner::run_all();
#endif
	neat::run_evolution(100);
	return 0;
}
