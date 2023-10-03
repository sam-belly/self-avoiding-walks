// Code for 2D merging.
// TO DO: implement a record of where a particle is absolutely, so that we can do a proper MSD curve.
// This requires keeping track of boundary crossings (need two integers).
// I implement helical boundary conditions.
//
// We will implement the memory as an unordered_map in this version.
// Since we don't care about infinite space, we can use the same grid index for the key.
// Wondering if the output could just be new sites, since any analysis we run can use previous data to create the list at any given time t.
 
#include <iostream>
#include "Xoshiro-cpp/XoshiroCpp.hpp"
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <array>
#include <cmath>
#include <vector>
#include <climits>
#include <unordered_map>
#include "HashGraph.hpp"

// I don't know what the right filename thing is, but whatever. It's here now. I shouldn't care about the size of the grid in the filename, anyway.
std::string filenameFromParams(std::string dir,
                               std::size_t Nwalkers,
                               int gap,
                               double beta,
                               long long Nsims) {
	
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << beta;
	std::string beta_s = stream.str();
	
	std::string target_name = dir +
	                          "/Nw_" + std::to_string(Nwalkers) +
	                          "_beta_" + beta_s +
	                          "_gap_" + std::to_string(gap) +
	                          "_Ns_" + std::to_string(Nsims) +
	                          ".csv";
	
	return target_name;
	
}

// Neighbour-finding functions.

// Finding neighbour indices is the most flexible approach.
int getIndexL(int Nsites, int x) {
	if(x == 0) {
		return Nsites - 1;
	} else {
		return x - 1;
	}
}

int getIndexR(int Nsites, int x) {
	if(x == Nsites - 1) {
		return 0;
	} else {
		return x + 1;
	}
}
// To get the upper and lower neighbours, you must add the gridLength, and then implement the boundary conditions.
int getIndexU(int gridLength, int Nsites, int x) {
	int newIndex = x + gridLength;
	if(newIndex > Nsites - 1) {
		newIndex -= Nsites;
	}
	return newIndex;
}

int getIndexD(int gridLength,int Nsites,int x) {
	int newIndex = x - gridLength;
	if(newIndex < 0) {
		newIndex += Nsites;
	}
	return newIndex;
}

// Neighbour order is (L,U,R,D). Not important as long as I am consistent.
std::array<int, 4> getNeighbourIndices(int gridLength, int Nsites, int x) {
	std::array<int, 4> neighbours;
	neighbours[0] = getIndexL(Nsites, x);
	neighbours[1] = getIndexU(gridLength, Nsites, x);
	neighbours[2] = getIndexR(Nsites, x);
	neighbours[3] = getIndexD(gridLength, Nsites, x);
	
	return neighbours;
}

// Returning integers for who has previously visited the site.
std::array<int, 4> getNeighbours(const std::unordered_map<int,int> &sites, std::array<int, 4> neighbours) {
	std::array<int, 4> newNeighbours;
	for(int i = 0; i < 4; i++) {
		if(sites.find(neighbours[i]) == sites.end()){
			newNeighbours[i] = 0;
		} else {
			newNeighbours[i] = sites.at(neighbours[i]);
		}
	}
	return newNeighbours;
}

// Walker time-stepping functions

// This function is the same as the 1D stuff.
float generateRandom(XoshiroCpp::Xoroshiro128Plus &gen,
                     std::uniform_real_distribution<float> &dist) {
	float rand = dist(gen);
	return rand;
}

// In this set-up, let beta be the relative weight of visiting a new site
std::array<float, 4> getWeights(std::array<int, 4> neighbours, float beta) {
	std::array<float, 4> weights;
	float normalisation = 0;
	
	for(int i = 0; i < 4; i++) {
		if(neighbours[i] == 0) {
			weights[i] = normalisation + beta;
			normalisation += beta;
		} else {
			weights[i] = normalisation + 1 - beta;
			normalisation += 1 - beta;
		}
	}
	
	for(float & w : weights) {
		w = w / normalisation;
	}
	
	return weights;
}

// L = 0, U = 1, R = 2, D = 3
int getDirection(std::array<float, 4> weights, float randValue) {
	
	int direction = 0;
	for(float w : weights) {
		if(randValue <= w) {
			break;
		} else {
			direction += 1;
		}
	}
	return direction;
}

// The actual update position thing is easy!
void updatePosition(int &x, int direction, std::array<int, 4> neighbours) {
	x = neighbours[direction];
}

// Note that particle ids need to start from 1, not 0.
void updateSites(int x, int id, std::unordered_map<int, int>& sites) {
	sites.insert({x,id});
}

// Cluster-merging logic.
// Idea: each site will have an integer based on which walker visited it first.
// Each time two territories become adjacent, we take note of this in a graph.
// Connected territories = connected graph.
// Will implement the graph using an adjacency list.

// For this I need the neighbours. When am I going to check if it is connected?
void updateGraph(Graph &g, int id, std::array<int, 4> neighbours) {
	for(int n : neighbours) {
		if(n != 0) {
			if(n != id) {
				g.AddEdge(id-1,n-1);
			}
		}
	}
}

void printSites(std::vector<int> sites, int gridLength) {
	for(int i = 0; i < gridLength; i++) {
		for(int j = 0; j < gridLength; j++) {
			std::cout<< sites[gridLength*i + j] << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	
	// Initialise the grid.
	const std::size_t Nsites = 268435456;
	int gridLength = 16384;

	// Initialise the walkers.
	float beta = 0.3;
	int Nwalkers = 2;
	int middle = Nsites/2 + gridLength/2;
	std::array<int,2> x0s = {middle-5,middle+5};
	long long Nsims = 1000000;
	
	// Initialise the savefile
	std::string filename = "data/brownian_many.csv";
	std::random_device rd;
	std::ofstream file;
	file.open(filename);
	
	std::unordered_map<int,int> global_frequency_table;
	
	#pragma omp parallel
	{
	// Set up the random engine.
	const std::uint64_t seed = rd();
	XoshiroCpp::Xoroshiro128Plus generator(seed);
	std::uniform_real_distribution<float> dist(0.0,1.0);
	
	std::unordered_map<int, int> thread_frequency_table;
	
	#pragma omp for schedule(dynamic)
	for(long long i = 0; i < Nsims; i++) {
	
		// Start at t = 0.
		int t = 0;
		std::array<int,2> xs = x0s;
	
		// Initialise the graph
		std::unordered_map<int, int> sites;
		Graph g(Nwalkers);
	
		while(!g.IsConnected()) {
			if (t == INT_MAX) {
				t = -1;
				break;
			}
			for (int i = 0; i < Nwalkers; i++) {
				std::array<int, 4> neighbours = getNeighbourIndices(gridLength,Nsites,xs[i]);
				std::array<int, 4> visitors = getNeighbours(sites,neighbours);
				std::array<float, 4> ws = getWeights(visitors,beta);
			
				float rand = generateRandom(generator,dist);
				int dir = getDirection(ws,rand);
				updatePosition(xs[i],dir,neighbours);
				updateSites(xs[i],i+1,sites);
			
				// May be that we get t+1 for the merging time but do we care??
				updateGraph(g,i+1,visitors);
			}
		
			t++;
		}
		
		if (thread_frequency_table.count(t) == 0) {
				thread_frequency_table[t] = 1;
		} else {
				thread_frequency_table[t] += 1;
		}
	}
	
	#pragma omp critical
	for (auto const &pair:thread_frequency_table) {
		if (global_frequency_table.count(pair.first) == 0) {
			global_frequency_table[pair.first] = pair.second;
		} else {
			global_frequency_table[pair.first] += pair.second;
		}
	}
	}
	
	for (auto const &pair:global_frequency_table) {
		file << pair.first << "," << pair.second << std::endl;
	}
	
	file.close();
			
	return 0;

}

