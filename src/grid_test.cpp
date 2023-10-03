#include "grid_walkers.hpp"
#include <iostream>

int main() {

	const std::size_t Nwalkers = 2;
	std::array<int, Nwalkers> X0s = {5,20};
	GridPopulation::GridWalkers<Nwalkers> walkers(X0s);
	std::array<int, 4> neighbours = walkers.NeighbourIndices(0);
	std::cout << neighbours[0] << "," 
	          << neighbours[1] << "," 
	          << neighbours[2] << ","
	          << neighbours[3] << std::endl;
	std::array<int, Nwalkers> inits{6,25};
	double msd = walkers.CalculateMSD(inits);
	std::cout << msd << std::endl;
	return 0;

}
