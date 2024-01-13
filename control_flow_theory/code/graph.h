#pragma once

#include <vector>
#include <set>
#include <utility>

#include "node.h"

struct Graph {
public:
    std::vector<Node> nodes;
    std::vector<int> weights;
    std::set<std::pair<int, int>> object_dependencies;
    std::vector<std::vector<bool>> dependency_matrix;
    std::vector<std::vector<bool>> descendants;
    int transporting_cost;
    int transporting_alpha;
    int loop_regularization;
};
