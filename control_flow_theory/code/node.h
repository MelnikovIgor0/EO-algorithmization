#pragma once

#include <vector>

struct Node {
    std::vector<int> execution_time;
    std::vector<int> object_ids;
    int color;
    std::vector<int> childs;
};