#pragma once

#include <fstream>
#include <vector>
#include <algorithm>

#include "node.h"
#include "graph.h"

Graph read_graph(std::istream &in) {
    Graph graph;
    int n, x;
    in >> n;
    graph.nodes.resize(n);

    for (int i = 0; i < n; ++i) {
        graph.nodes[i].execution_time.resize(2);
        in >> graph.nodes[i].execution_time[0] >> graph.nodes[i].execution_time[1];
        in >> x;
        graph.nodes[i].object_ids.resize(x);

        for (int j = 0; j < x; ++j) {
            in >> graph.nodes[i].object_ids[j];
        }

        in >> graph.nodes[i].color;
        in >> x;
        graph.nodes[i].childs.resize(x);

        for (int j = 0; j < x; ++j) {
            in >> graph.nodes[i].childs[j];
        }
    }

    in >> n;
    graph.weights.resize(n);
    for (int i = 0; i < n; ++i) {
        in >> graph.weights[i];
    }

    in >> graph.transporting_cost >> graph.transporting_alpha >> graph.loop_regularization;
    return graph;
}