#pragma once

#include <vector>

struct Node {
    std::vector<int> execution_time;
    std::vector<int> vtx_read;
    std::vector<int> vtx_write;
    int color;
    std::vector<int> childs;
};