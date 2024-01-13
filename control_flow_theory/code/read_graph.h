#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <unordered_set>
#include <utility>

#include "node.h"
#include "graph.h"

#include "nlohman/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

Graph read_graph(const std::string& filename) {
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();
    json input = json::parse(content);

    Graph graph;

    graph.nodes.resize(input["nodes"].size());
    graph.weights.resize(input["objects"].size());
    graph.transporting_cost = input["transporting_cost"].get<int>();
    graph.transporting_alpha = input["transporting_alpha"].get<int>();
    graph.loop_regularization = input["loop_regularization"].get<int>();

    //graph.object_dependencies = input["dependencies"].get<std::unordered_set<std::pair<int,int>>>();

    for (int i = 0; i < graph.nodes.size(); ++i) {
        int idx = input["nodes"][i]["id"].get<int>();
        int exec_a = input["nodes"][i]["exec_a"].get<int>();
        int exec_b = input["nodes"][i]["exec_b"].get<int>();
        std::vector<int> childs = input["nodes"][i]["childs"].get<std::vector<int>>();
        std::vector<int> vtx_read = input["nodes"][i]["vtx_read"].get<std::vector<int>>();
        std::vector<int> vtx_write = input["nodes"][i]["vtx_write"].get<std::vector<int>>();
        graph.nodes[idx].execution_time = std::vector<int>{exec_a, exec_b};
        graph.nodes[idx].color = 0;
        graph.nodes[idx].childs = childs;
        graph.nodes[idx].vtx_read = vtx_read;
        graph.nodes[idx].vtx_write = vtx_write;
    }

    for (int i = 0; i < graph.weights.size(); ++i) {
        int idx = input["objects"][i]["id"];
        int weight = input["objects"][i]["weight"];
        graph.weights[idx] = weight;
    }

    return graph;
}
