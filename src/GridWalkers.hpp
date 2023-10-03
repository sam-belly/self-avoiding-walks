// A class to hold walkers on a grid.
// It provides sets of neighbours for an individual walker.
// It will also update a set of walkers according to an array of new positions.
// Finally, given a vector of initial positions, it will calculate the MSD at the time step.
// In this way, all mentions of patches and gridLength are kept in one place.
// I will still need to keep a record of Nsites somewhere if I want to know visited site density.
// This current incarnation prints out positions as indices, not (x,y) pairs.
//
//
// I need to have constructors, or I'm not going to be able to initialise this shit.
#ifndef GRIDWALKERS_H
#define GRIDWALKERS_H

#include <array>
#include <cmath>
#include <iostream>

namespace GridPopulation
{
template<std::size_t Nwalkers>
class GridWalkers {
	private:
		const int gridLength;
		const int Nsites;

		// Set-up default initial positions at 0.
		std::array<int, Nwalkers> Positions;
		std::array<int, Nwalkers> PatchesX;
		std::array<int, Nwalkers> PatchesY;
		
		int NeighbourIndexL(int x);
		int NeighbourIndexU(int x);
		int NeighbourIndexR(int x);
		int NeighbourIndexD(int x);
		
	public:
		GridWalkers() : gridLength(32), Nsites(1024), Positions(), PatchesX(), PatchesY() {Positions.fill(500); PatchesX.fill(0); PatchesY.fill(0);};
		GridWalkers(std::array<int,Nwalkers> x0s) : gridLength(32), Nsites(1024), Positions(x0s), PatchesX(), PatchesY() {
		PatchesX.fill(0);
		PatchesY.fill(0);
		};
		GridWalkers(int L, std::array<int,Nwalkers> x0s) : gridLength(L), Nsites(L*L), Positions(x0s), PatchesX(), PatchesY() {
		PatchesX.fill(0);
		PatchesY.fill(0);
		};
		std::array<int, 4> NeighbourIndices(int id);
		void updatePositions(const std::array<int, Nwalkers>& newPositions);
		double CalculateMSD(const std::array<int, Nwalkers>& initialPositions);
		void PrintPositions(bool endline);
		
};

template<std::size_t Nwalkers>
int GridWalkers<Nwalkers>::NeighbourIndexL(int x) {
	if(x == 0) {
		return Nsites - 1;
	} else {
		return x - 1;
	}
}

template<std::size_t Nwalkers>
int GridWalkers<Nwalkers>::NeighbourIndexU(int x) {
	int newIndex = x + gridLength;
	if(newIndex > Nsites - 1) {
		newIndex -= Nsites;
	}
	
	return newIndex;
}

template<std::size_t Nwalkers>
int GridWalkers<Nwalkers>::NeighbourIndexR(int x) {
	if(x == Nsites - 1) {
		return 0;
	} else {
		return x + 1;
	}
}

template<std::size_t Nwalkers>
int GridWalkers<Nwalkers>::NeighbourIndexD(int x) {

	int newIndex = x - gridLength;
	if(newIndex < 0) {
		newIndex += Nsites;
	}
	
	return newIndex;
}

// Here I use the id tag, which must start from 1. Hence we need id - 1 in the positions.
template<std::size_t Nwalkers>
std::array<int, 4> GridWalkers<Nwalkers>::NeighbourIndices(int id) {

	std::array<int, 4> indices;
	int x = Positions[id-1];

	indices.at(0) = NeighbourIndexL(x);
	indices.at(1) = NeighbourIndexU(x);
	indices.at(2) = NeighbourIndexR(x);
	indices.at(3) = NeighbourIndexD(x);
	
	return indices;
}

template<std::size_t Nwalkers>
void GridWalkers<Nwalkers>::updatePositions(const std::array<int, Nwalkers>& newPositions) {

	for(int i = 0; i < Nwalkers; i++) {
		// Update the position.
		int xi = Positions[i];
		int xf = newPositions[i];
		// Check to see if there has been a crossing. If so update patches.
		if((xf % gridLength == gridLength - 1) && (xi % gridLength == 0)) {
			// xf is on the right, xi is on the left, so moved to left.
			PatchesX[i] -= 1;
		}
		if((xf % gridLength == 0) && (xi % gridLength == gridLength - 1)) {
			// xf is on the left, xi is on the right, so moved to right.
			PatchesX[i] += 1;
		}
		if((xf > Nsites - gridLength - 1) && (xi < gridLength)) {
			// xf on top, and xi on bottom, so moved down.
			PatchesY[i] -= 1;
		}
		if((xf < gridLength) && (xi > Nsites - gridLength - 1)) {
			// xf low, and xi high, so moved up.
			PatchesY[i] += 1;
		}
	}
	
	// Now update the positions.
	Positions = newPositions;
}

template<std::size_t Nwalkers>
double GridWalkers<Nwalkers>::CalculateMSD(const std::array<int, Nwalkers>& initialPositions) {

	double totalSD = 0;

	for(int i = 0; i < Nwalkers; i++) {
		int x0 = initialPositions[i] % gridLength;
		int y0 = floor(initialPositions[i]/gridLength);
		
		int xf = (Positions[i] % gridLength) + PatchesX[i]*gridLength;
		int yf = floor(Positions[i]/gridLength) + PatchesY[i]*gridLength;
		
		totalSD += pow(xf-x0,2) + pow(yf-y0,2);
	}
	
	double MSD = totalSD / Nwalkers;
	
	return MSD;
	
}

// This PrintPositions does close the line.
// It could be complicated if I want to do a system printout.
// But maybe I can make it so that the walkers print out last.
template<std::size_t Nwalkers>
void GridWalkers<Nwalkers>::PrintPositions(bool endline) {

	for (int x : Positions) {
		std::cout << x << ",";
	}
	
	if(endline) {
	std::cout << std::endl;
	}

}
}
#endif
