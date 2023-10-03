# self-avoiding-walks
Some code for simulating SATWs (on-lattice).

The py folder contains some analysis files. I have some more on another folder, mainly focussed on analysing first passage time statistics.

merge_hash.cpp and merge_parallel.cpp are first passage experiments, looking at the merging time of territories. This is interesting for me for a percolation project.

SATWs.hpp is the meat of the SATW simulation. I decided to put all of it together in a class, so that it could be used between different simulation types.

I don't have a good example of using SATWs.hpp yet, because the merging code was already written out (essentially it's the same but written without the class).
The class can calculate MSD but can't calculate the increments. For that, I think it would probably be best to just chuck out the positions of the walkers at each time point.
Generate the data, and analyse it afterwards.

Maybe there are cleverer ways of doing it, but I haven't looked through Alex's code to see if he found some sneaky way.

GridWalkers.hpp implements helical boundary conditions (including keeping track of which "patch" a walker is on for the MSD calculation). This is an important point. I reference my lattice sites with one index, rather than an (x,y) tuple.
I guess the logic at one point was that when storing visited sites in a hashmap, it was easier to store a bare site index.
There may be some value in using an (x,y) tuple. In particular, it's easier to implement an open simulation without periodic boundary conditions.
Although it is an interesting exercise to create open boundary conditions using one index - the best idea I could come up with was to have the index spiralling out from the origin.

HashGraph.hpp, Graph.hpp, and GraphDFS.hpp are all about implementing depth-first-search and storing network connectivity when worried about percolation.
An SATW object contains a connectivity graph - if you don't care about connectivity, it's better not to update the graph (UpdateWalkersNoGraph) because the graph slows down the sim for large particle numbers.

If it's not clear, I'm happy to help.
