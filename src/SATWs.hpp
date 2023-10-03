// To-do today: write the constructors.
// Need to initialise the generator, and the Walkers object.
// Otherwise, the default constructors are fine.
//
// I probably also need to initialise sites with the initial positions as well.
// The merges are updated after the first time step - they lag behind by one time step, I think.
#ifndef SATWS_H
#define SATWS_H

#include <cmath>
#include <random>
#include <array>
#include <unordered_map>
#include "HashGraph.hpp"
#include "GridWalkers.hpp"
#include <vector>

namespace SATW
{
template<class Gen, std::size_t Nwalkers>
class SATs {
	private:
		// Walkers and grid details.
		GridPopulation::GridWalkers<Nwalkers> Walkers;

		// Visited sites and connectedness.
		std::unordered_map<int, int> sites;
		
		// new_sites exists for economy of printing out state changes rather than state.
		std::vector<int> new_sites;
		
		// A graph for monitoring how connected we are.
		HashGraph::Graph merges;
		
		// Random walk details.
		Gen generator;
		std::uniform_real_distribution<float> distribution;
		float beta = 0.5;
		
		// These four functions are needed to get the next site for each particle.
		std::array<int, 4> NeighbourIndices(int id);
		std::array<int, 4> Neighbours(std::array<int, 4> indices);
		std::array<float, 4> Weights(std::array<int, 4> neighbours);
		int StepDirection(std::array<float, 4> weights);
		
		// Updating the hashmap and the merges graph
		void UpdateSites(int x, int id);
		void UpdateGraph(int id, std::array<int, 4> neighbours);
		
	public:
		// Constructors.
		SATs(std::uint64_t seed, std::array<int, Nwalkers> x0s) : Walkers(x0s), sites(), new_sites(), merges(Nwalkers), generator(seed), distribution(0.0,1.0) {
			//sites.reserve(1024);
			for (int i = 0; i < Nwalkers; i++) {
				sites.insert({x0s[i],i+1});
			}
		};
		SATs(std::uint64_t seed, int L, std::array<int, Nwalkers> x0s) : Walkers(L,x0s), sites(), new_sites(), generator(seed), merges(Nwalkers), distribution(0.0,1.0) {
			//sites.reserve(L*L);
			for (int i = 0; i < Nwalkers; i++) {
				sites.insert({x0s[i],i+1});
			}
		};
		SATs(std::uint64_t seed, float b, std::array<int, Nwalkers> x0s) : Walkers(x0s), sites(), new_sites(), merges(Nwalkers), generator(seed), distribution(0.0,1.0), beta(b) {
			//sites.reserve(1024);
			for (int i = 0; i < Nwalkers; i++) {
				sites.insert({x0s[i],i+1});
			}
		};
		SATs(std::uint64_t seed, float b, int L, std::array<int, Nwalkers> x0s) : Walkers(L,x0s), sites(), new_sites(), generator(seed), merges(Nwalkers), distribution(0.0,1.0), beta(b) {
			//sites.reserve(L*L);
			for (int i = 0; i < Nwalkers; i++) {
				sites.insert({x0s[i],i+1});
			}
		};
		void UpdateWalkers();
		bool IsConnected();
		int SitesVisited();
		// Get the MSD from some inputted initial positions.
		double CalculateMSD(std::array<int, Nwalkers> x0s);
		//void PrintInitialState();
		//void PrintUpdates();
		void PrintPositions();
		
};

template<class Gen, std::size_t Nwalkers>
std::array<int, 4> SATs<Gen,Nwalkers>::NeighbourIndices(int id) {
	return Walkers.NeighbourIndices(id);
}

template<class Gen, std::size_t Nwalkers>
std::array<int, 4> SATs<Gen,Nwalkers>::Neighbours(std::array<int, 4> indices) {

	std::array<int, 4> neighbours;
	for(int i = 0; i < 4; i++) {
		if(sites.find(indices[i]) == sites.end()){
			neighbours[i] = 0;
		} else {
			neighbours[i] = sites.at(indices[i]);
		}
	}
	
	return neighbours;
}

template<class Gen, std::size_t Nwalkers>
std::array<float, 4> SATs<Gen,Nwalkers>::Weights(std::array<int, 4> neighbours) {

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

template<class Gen, std::size_t Nwalkers>
int SATs<Gen,Nwalkers>::StepDirection(std::array<float, 4> weights) {
	
	float randValue = distribution(generator);
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

// Note that particle ids need to start from 1, not 0.
// This is because 0 indicates an empty site.
template<class Gen, std::size_t Nwalkers>
void SATs<Gen,Nwalkers>::UpdateSites(int x, int id) {
	auto [iterator,ok] = sites.insert({x,id});
	if(ok) {
		new_sites.push_back(x);
	}
}

// This is my problem function at the moment. I don't really know why.
template<class Gen, std::size_t Nwalkers>
void SATs<Gen,Nwalkers>::UpdateGraph(int id, std::array<int, 4> neighbours) {
	for(int n : neighbours) {
		if((n != 0) && (n != id)) {
			merges.AddEdge(id-1,n-1);
		}
	}
}

template<class Gen, std::size_t Nwalkers>
void SATs<Gen,Nwalkers>::UpdateWalkers() {
	new_sites.clear();
	std::array<int, Nwalkers> new_positions;
	std::array<int, 4> indices;
	std::array<int, 4> neighbours;
	std::array<float, 4> weights;
	
	for (int i = 0; i < Nwalkers; i++) {
	
		int id = i + 1;
		indices = NeighbourIndices(id);
		neighbours = Neighbours(indices);
		weights = Weights(neighbours);
		
		int dir = StepDirection(weights);
		new_positions[i] = indices[dir];

		UpdateSites(new_positions[i], id);
		//UpdateGraph(id, neighbours);
		
	}
	
	Walkers.updatePositions(new_positions);
}

template<class Gen, std::size_t Nwalkers>
double SATs<Gen,Nwalkers>::CalculateMSD(std::array<int, Nwalkers> x0s) {
	return Walkers.CalculateMSD(x0s);
}

template<class Gen, std::size_t Nwalkers>
bool SATs<Gen,Nwalkers>::IsConnected() {
	return merges.IsConnected();
}

template<class Gen, std::size_t Nwalkers>
int SATs<Gen,Nwalkers>::SitesVisited() {
	return sites.size();
}

template<class Gen, std::size_t Nwalkers>
void SATs<Gen,Nwalkers>::PrintPositions() {
	Walkers.PrintPositions(true);
}

}

#endif
