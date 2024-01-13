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
#include "check_coloring.h"

using namespace std;

int get_bit(int mask, int i) {
    return ((mask >> i) & 1);
}

pair<string, double> full_search(Graph& graph) {
    string best_coloring = "";
    double best_laboriousness = 1e100;
    double laboriousness;
    int limit = (1 << graph.nodes.size()) / 100;
    int good_colorings = 0;
    for (int mask = 0; mask < (1 << graph.nodes.size()); ++mask) {
        if (mask % limit == 0) {
            cout << (mask / limit) << '/' << 100 << '\n';
        }
        for (int i = 0; i < graph.nodes.size(); ++i) {
            if (get_bit(mask, i)) {
                graph.nodes[i].color = 0;
            } else {
                graph.nodes[i].color = 1;
            }
        }
        if (!check_coloring(graph)) {
            continue;
        }
        ++good_colorings;
        laboriousness = evaluate_graph(graph);
        if (laboriousness < best_laboriousness) {
            best_laboriousness = laboriousness;
            best_coloring = "";
            for (int i = 0; i < graph.nodes.size(); ++i) {
                if (get_bit(mask, i)) {
                    best_coloring.push_back('A');
                } else {
                    best_coloring.push_back('B');
                }
            }
        }
    }
    return make_pair(best_coloring, best_laboriousness);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 0;
    }
    Graph graph = read_graph(argv[1]);
    pair<string, double> result = full_search(graph);
    cout << result.first << ' ' << result.second << '\n';
    return 0;
}
