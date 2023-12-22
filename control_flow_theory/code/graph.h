#pragma once

#include <vector>

#include "node.h"

struct Graph {
    std::vector<Node> nodes;
    std::vector<int> weights;
    int transporting_cost;
    int transporting_alpha;
    int loop_regularization;
};