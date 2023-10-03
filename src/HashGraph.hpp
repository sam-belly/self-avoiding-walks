#ifndef HASHGRAPH_H
#define HASHGRAPH_H

#include<iostream>
#include<vector>
#include<stack>
#include<unordered_map>

// This is a graph which doesn't allow duplicate edges.
// This is to prevent the structure getting too big for walkers on different territories.
namespace HashGraph
{
class Graph {

    private:

        int nodes;
        std::unordered_map<int, int> *adjlist;
        std::vector<bool> visited;

    public:

        Graph() {
        }

        Graph (int nodes) { // Allocate resources 
            adjlist = new std::unordered_map<int, int> [nodes];
            visited.resize(nodes, false);
            this->nodes = nodes;
        }

        ~Graph () { // Free allocated resources
            delete [] adjlist;
        }

        void AddEdge (int src, int dst) {
            adjlist[src].insert({dst,1});
            adjlist[dst].insert({src,1});
        }

        // DFS recursive
        void DFS (int src) {
            visited[src] = true;
            std::cout << src << " ";
            for (auto& adj_node : adjlist[src]) {
                if (!visited[adj_node.first]) {
                    DFS(adj_node.first);
                }
            }
        }
        
        // DFS
        void DFS_count (int src, int & count) {
            ++count;
            visited[src] = true;
            for (auto& adj_node : adjlist[src]) {
                if (!visited[adj_node.first]) {
                    DFS_count(adj_node.first,count);
                }
            }
        }
        
        bool IsConnected() {
            bool connected = false;
            int count = 0;
            DFS_count(0,count);
            if(count == visited.size()) {
                connected = true;
            }
            MarkUnvisited();
            return connected;
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
                    if (!visited[adj_node.first]) {
                        visited[adj_node.first] = true;
                        stk.push(adj_node.first);
                    }
                }
            }
        }

        // Mark nodes unvisited for next traversal
        void MarkUnvisited () {
            fill(visited.begin(), visited.end(), false);
        }
};
}
#endif
