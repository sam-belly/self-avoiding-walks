#ifndef GRAPHDFS_H
#define GRAPHDFS_H

#include<iostream>
#include<list>
#include<vector>
#include<stack>


// Note that we do not have freedom to choose node labels here.
// Node identities always start from 0: they are an array index.
// One possible problem is that we may end up with adjacency lists with duplicate items...
// I probably do care about duplicate edges. Because if I walk on somebody else's territory for
// 1000 timesteps... That would be a big graph with not a lot of information.
// So we're going to try and use an unordered_map

class Graph {

    private:

        int nodes;
        std::list<int> *adjlist;
        std::vector<bool> visited;

    public:

        Graph() {
        }

        Graph (int nodes) { // Allocate resources 
            adjlist = new std::list<int> [nodes];
            visited.resize(nodes, false);
            this->nodes = nodes;
        }

        ~Graph () { // Free allocated resources
            delete [] adjlist;
        }

        void AddEdge (int src, int dst) {
            adjlist[src].push_back(dst);
            adjlist[dst].push_back(src);
        }

        // DFS recursive
        void DFS (int src) {
            visited[src] = true;
            std::cout << src << " ";
            for (auto& adj_node : adjlist[src]) {
                if (!visited[adj_node]) {
                    DFS(adj_node);
                }
            }
        }
        
        // DFS counting of nodes visited
        void DFS_count (int src, int & count) {
            ++count;
            visited[src] = true;
            for (auto& adj_node : adjlist[src]) {
                if (!visited[adj_node]) {
                    DFS_count(adj_node,count);
                }
            }
        }
        
        // Checks if a graph is connected by looking at DFS count from mode src.
        bool IsConnected() {
            bool connected = false;
            int count = 0;
            DFS_count(0,count);
            if(count == visited.size()) {
                connected = true;
            }
            return connected;
            MarkUnvisited();
        }

        // DFS iterative
        void DFS_Iterative (int src) {

            std::stack<int> stk;
            visited[src] = true;
            stk.push(src);

            while (!stk.empty()) {
                src = stk.top();
                stk.pop();
                std::cout << src << " ";
                for (auto &adj_node : adjlist[src]) {
                    if (!visited[adj_node]) {
                        visited[adj_node] = true;
                        stk.push(adj_node);
                    }
                }
            }
        }

        // Mark nodes unvisited for next traversal
        void MarkUnvisited () {
            fill(visited.begin(), visited.end(), false);
        }
};
