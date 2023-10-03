// This test file seems to suggest that the valgrind problems do not come from GridWalkers.
// So maybe it is the initialisation of the Walkers within the SATs.
#include "GridWalkers.hpp"
#include "Xoshiro-cpp/XoshiroCpp.hpp"
#include <array>
#include <iostream>

int main() {

	std::array<int, 2> x0s = {33,44};
	GridPopulation::GridWalkers<2> sats(x0s);
	
	std::array<int, 4> indices = sats.NeighbourIndices(2);
	std::cout << indices.at(0) << "," 
                  << indices.at(1) << ","
                  << indices.at(2) << ","
                  << indices.at(3)
                  << std::endl;
	
	std::array<int, 2> new_positions = {43,54};
	sats.updatePositions(new_positions);
	
	indices = sats.NeighbourIndices(1);
	std::cout << indices.at(0) << "," 
                  << indices.at(1) << ","
                  << indices.at(2) << ","
                  << indices.at(3)
                  << std::endl;
                  
	return 0;

}
