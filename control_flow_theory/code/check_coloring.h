#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <unordered_set>

#include "graph.h"
#include "node.h"
#include "read_graph.h"

using namespace std;

bool check_terminals(const Graph& graph, vector<bool>& used, int node_id) {
    used[node_id] = true;
    bool is_terminal = true;
    for (int child_id : graph.nodes[node_id].childs) {
        if (!used[child_id]) {
            if (!check_terminals(graph, used, child_id)) {
                return false;
            }
            is_terminal = false;
        }
    }
    if (is_terminal) {
        return graph.nodes[node_id].color == 0;
    }
    return true;
}

void set_descendants(Graph& graph, vector<bool>& used, int subroot, int node_id) {
    used[node_id] = true;
    graph.descendants[subroot][node_id] = true;
    for (int child_id : graph.nodes[node_id].childs) {
        if (!used[child_id]) {
            set_descendants(graph, used, subroot, child_id);
        }
    }
}

void set_dependency_matrix(Graph& graph, const vector<vector<int>>& g, vector<bool>& used, int subroot_id, int object_id) {
    used[object_id] = true;
    graph.dependency_matrix[subroot_id][object_id] = true;
    for (int child_id : g[object_id]) {
        if (!used[child_id]) {
            set_dependency_matrix(graph, g, used, subroot_id, child_id);
        }
    }
}

void set_dependency_matrix(Graph& graph) {
    vector<vector<int>> g(graph.weights.size());
    for (const pair<int, int>& p : graph.object_dependencies) {
        g[p.first].push_back(p.second);
    }
    graph.dependency_matrix.assign(graph.weights.size(), vector<bool>(graph.weights.size(), false));
    vector<bool> used;
    for (int i = 0; i < graph.weights.size(); ++i) {
        used.assign(graph.weights.size(), false);
        set_dependency_matrix(graph, g, used, i, i);
    }
}

void set_descendants(Graph& graph) {
    vector<bool> used;
    graph.descendants.assign(graph.nodes.size(), vector<bool>(graph.nodes.size(), false));
    for (int i = 0; i < graph.nodes.size(); ++i) {
        used.assign(graph.nodes.size(), false);
        set_descendants(graph, used, i, i);
    }
}

bool check_coloring(Graph& graph) {
    if (graph.nodes[0].color) {
        return false;
    }
    vector<bool> used(graph.nodes.size(), false);
    if (!check_terminals(graph, used, 0)) {
        return false;
    }

    if (graph.descendants.size() == 0) {
        set_descendants(graph);
    }

    if (graph.dependency_matrix.size() == 0) {
        set_dependency_matrix(graph);
    }

    for (int i = 0; i < graph.nodes.size(); ++i) {
        for (int j = 0; j < graph.nodes.size(); ++j) {
            for (const vector<int>& close1 : {graph.nodes[i].vtx_read, graph.nodes[i].vtx_write}) {
                for (const vector<int>& close2 : {graph.nodes[j].vtx_read, graph.nodes[j].vtx_write}) {
                    for (int object_close1 : close1) {
                        for (int object_close2 : close2) {
                            if (graph.descendants[object_close1][object_close2] && graph.dependency_matrix[object_close1][object_close2]) {
                                if (graph.nodes[object_close1].color != graph.nodes[object_close2].color) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}
