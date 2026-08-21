#pragma once

#include <cstdint>
#include <vector>

struct Node {
    uint8_t byte;
    uint64_t count;

    std::vector<Node> children;

    bool isLeaf;
};