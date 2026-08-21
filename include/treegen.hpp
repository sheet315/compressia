#pragma once

#include <cstdint>
#include <vector>

struct Node {
    uint8_t byte;
    uint64_t count;

    std::vector<Node> children;

    bool isLeaf;

    Node(uint8_t _byte, uint64_t _count, bool _isLeaf) {
        byte = _byte;
        count = _count;
        isLeaf = _isLeaf;
    }
};