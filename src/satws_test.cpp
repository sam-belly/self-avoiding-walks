#include "SATWs.hpp"
#include "Xoshiro-cpp/XoshiroCpp.hpp"
#include <array>
#include <iostream>

int main() {

	std::random_device rd;
	const std::uint64_t seed = rd();
	std::array<int, 3> x0s = {21,31,750};
	SATW::SATs<XoshiroCpp::Xoroshiro128Plus, 3> sats(seed, x0s);
	int t = 0;
	
	while(!sats.IsConnected()) {
	sats.UpdateWalkers();
	t++;
	}
	
	std::cout << "Sites visited: " << sats.SitesVisited() << std::endl;
	std::cout << "The current MSD: " << sats.CalculateMSD(x0s) << std::endl;
	std::cout << "Time to Connection: " << t << std::endl;
	
	return 0;

}
