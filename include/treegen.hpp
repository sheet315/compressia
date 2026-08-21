#pragma once

#include <cstdint>
#include <vector>

struct Node {
    uint8_t  byte;
    uint64_t count;

    std::vector<Node> children;

    bool isLeaf;

    Node(uint8_t _byte, uint64_t _count, bool _isLeaf) {
        byte   = _byte;
        count  = _count;
        isLeaf = _isLeaf;
    }

    Node() {}
};

std::vector<Node>    genNodes(std::array<std::pair<uint8_t, uint64_t>, 256>& symbols);
Node                 genTree(std::vector<Node> nodes);
std::vector<uint8_t> serializeTree(Node& tree);
bool                 getCode(Node& node, uint8_t target, std::string& code);