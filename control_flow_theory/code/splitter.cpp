#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <unordered_set>

#include "evaluation_function.h"
#include "graph.h"
#include "node.h"
#include "read_graph.h"

using namespace std;

void try_relax_edge(Graph& graph, unordered_set<int>& recolored, int root_id, int node_id) {
    graph.nodes[node_id].color = 1 - graph.nodes[node_id].color;
    recolored.insert(node_id);
    for (int child_id : graph.nodes[node_id].childs) {
        if (graph.nodes[child_id].color != graph.nodes[root_id].color) {
            try_relax_edge(graph, recolored, root_id, child_id);
        }
    }
}

void dfs(Graph& graph, vector<bool> &used, int node_id) {
    used[node_id] = true;
    double best_value = 1e20;
    for (int child_id : graph.nodes[node_id].childs) {
        unordered_set<int> recolored;
        try_relax_edge(graph, recolored, node_id, child_id);
        double value = evaluate_graph(graph);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 0 || argc > 2) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 0;
    }
    Graph graph;
    if (argc == 2) {
        ifstream fin(argv[1]);
        graph = read_graph(fin);
        fin.close();
    } else {
        graph = read_graph(cin);
    }

    return 0;
}