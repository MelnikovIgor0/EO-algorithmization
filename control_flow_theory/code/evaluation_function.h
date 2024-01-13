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

// #define DEBUG

using namespace std;

void _find_objects_read_and_write(const Graph& graph, vector<bool> &used, unordered_set<int>& object_ids, int node_id) {
    used[node_id] = true;

    for (int object_id : graph.nodes[node_id].vtx_read) {
        object_ids.insert(object_id);
    }

    for (int object_id : graph.nodes[node_id].vtx_write) {
        object_ids.insert(object_id);
    }

    for (int child_id : graph.nodes[node_id].childs) {
        if (!used[child_id] && graph.nodes[node_id].color == graph.nodes[child_id].color) {
            _find_objects_read_and_write(graph, used, object_ids, child_id);
        }
    }
}

void _find_objects_write(const Graph& graph, vector<bool> &used, unordered_set<int>& object_ids, int node_id) {
    used[node_id] = true;

    for (int object_id : graph.nodes[node_id].vtx_write) {
        object_ids.insert(object_id);
    }

    for (int child_id : graph.nodes[node_id].childs) {
        if (!used[child_id] && graph.nodes[node_id].color == graph.nodes[child_id].color) {
            _find_objects_write(graph, used, object_ids, child_id);
        }
    }
}

int _evaluate_vertex(const Graph& graph, const unordered_set<int>& used_objects_ids, int estimator_id, int parent_id, int child_id) {
    if (graph.nodes[parent_id].color == graph.nodes[child_id].color) {
        return graph.nodes[parent_id].execution_time[graph.nodes[parent_id].color];
    }

    vector<bool> used(graph.nodes.size(), false);
    unordered_set<int> object_ids;
    if (graph.nodes[parent_id].color == 0 && graph.nodes[child_id].color == 1) {
        _find_objects_read_and_write(graph, used, object_ids, child_id);
    } else {
        _find_objects_write(graph, used, object_ids, estimator_id);
    }
    int total_sum = 0;

    for (int object_id : object_ids) {
        if (used_objects_ids.count(object_id)) {
            total_sum += graph.weights[object_id];
        }
    }
    
    return total_sum * graph.transporting_alpha + graph.transporting_cost;
}

double _dfs(const Graph& graph, unordered_set<int>& used_objects_ids, vector<bool>& used, vector<double>& cache, int estimator_id, int node_id, double accumulated_time) {
    if (cache[node_id] > 0) {
        return cache[node_id];
    }

    used[node_id] = true;
    double sum_time = 0;
    int dfs_count = 0;

    for (int child_id : graph.nodes[node_id].childs) {
        if (!used[child_id]) {
            ++dfs_count;
            
            #ifdef DEBUG
            cout << "evaluation: " << node_id << ' ' << child_id << ' ' << _evaluate_vertex(graph, used_objects_ids, node_id, child_id) << endl;
            #endif

            unordered_set<int> add;

            for (int object_id : graph.nodes[child_id].vtx_read) {
                if (!used_objects_ids.count(object_id)) {
                    used_objects_ids.insert(object_id);
                    add.insert(object_id);
                }
            }

            for (int object_id : graph.nodes[child_id].vtx_write) {
                if (!used_objects_ids.count(object_id)) {
                    used_objects_ids.insert(object_id);
                    add.insert(object_id);
                }
            }

            int new_estimator_id = estimator_id;
            if (graph.nodes[estimator_id].color != graph.nodes[child_id].color) {
                new_estimator_id = child_id;
            }

            sum_time += _dfs(graph, used_objects_ids, used, cache, new_estimator_id, child_id, accumulated_time + _evaluate_vertex(graph, used_objects_ids, estimator_id, node_id, child_id));

            for (int object_id : add) {
                used_objects_ids.erase(object_id);
            }
        } else {
            #ifdef DEBUG
            cout << "loop regularization (" << node_id << ';' << child_id << "): " << _evaluate_vertex(graph, used_objects_ids, node_id, child_id) * graph.loop_regularization << endl;
            #endif
            
            sum_time += _evaluate_vertex(graph, used_objects_ids, estimator_id, node_id, child_id) * graph.loop_regularization;
        }
    }
    
    if (!dfs_count) {
        return accumulated_time;
    }
    cache[node_id] = sum_time / dfs_count;
    return cache[node_id];
}

double evaluate_graph(const Graph& graph) {
    vector<bool> used(graph.nodes.size(), false);
    vector<double> cache(graph.nodes.size(), -1);
    unordered_set<int> used_object_ids;
    cache[0] = 0;
    return _dfs(graph, used_object_ids, used, cache, 0, 0, 0);
}
