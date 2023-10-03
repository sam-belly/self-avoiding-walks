#include "SATWs.hpp"
#include "Xoshiro-cpp/XoshiroCpp.hpp"
#include <iostream>
#include <array>
#include <fstream>
#include <vector>

int main() {
	
	std::random_device rd;
	const std::uint64_t seed = rd();
	std::array<int, 4096> x0s;
	for (int i = 0; i < 64; i++) {
		for(int j = 0; j < 64; j++) {
			x0s[64*i + j] = 16*1024*i + 16*j;
		}
	}
	SATW::SATs<XoshiroCpp::Xoroshiro128Plus, 4096> sats(seed, 0.1, 1024, x0s);
	
	std::string filename = "data/msd_beta010_4096lattice1024.csv";
	std::ofstream file;
	file.open(filename);
	
	for (int t = 1; t < 1000000; t++) {
	
		sats.UpdateWalkers();
		double msd = sats.CalculateMSD(x0s);
		int sites = sats.SitesVisited();
		bool connected = sats.IsConnected();
		file << t << "," << msd << "," << sites << "," << connected << "\n";
	
	}
	
	std::cout << std::endl;
	
	file.close();
	
	return 0;
	
}
