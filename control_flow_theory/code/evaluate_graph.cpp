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
    cout << evaluate_graph(graph) << endl;
    return 0;
}