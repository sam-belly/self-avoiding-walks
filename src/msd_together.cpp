#include "SATWs2.hpp"
#include "Xoshiro-cpp/XoshiroCpp.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <vector>

int main() {
	
	std::random_device rd;
	const std::uint64_t seed = rd();
	std::array<int, 10> x0s;
	int sites = 1024 * 1024;
	int middle = sites / 2 + 512;
	x0s.fill(middle);
	
	SATW::SATs<XoshiroCpp::Xoroshiro128Plus, 10> sats(seed, 0.1, 1024, x0s);
	
	std::string filename = "data/msd_beta010_10together.csv";
	std::ofstream file;
	file.open(filename);
	
	for (int t = 1; t < 100000; t++) {
	
		sats.UpdateWalkersNoGraph();
		double msd = sats.CalculateMSD(x0s);
		int sites = sats.SitesVisited();
		file << t << "," << msd << "," << sites << "\n";
	
	}
	
	std::cout << std::endl;
	
	file.close();
	
	return 0;
	
}
