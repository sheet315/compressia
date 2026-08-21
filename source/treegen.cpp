#include <array>
#include <string>

#include "../include/treegen.hpp"

std::vector<Node> genNodes(std::array<std::pair<uint8_t, uint64_t>, 256>& symbols) {
    std::vector<Node> nodes;
    for (size_t i = 0; i < 256; i++) {
        nodes.emplace_back(symbols[i].first, symbols[i].second, true);
    }

    return nodes;
}

Node genTree(std::vector<Node> nodes) {
    while (nodes.size() > 1) {
        size_t smallest = 0;
        for (size_t i = 1; i < nodes.size(); i++) {
            if (nodes[i].count < nodes[smallest].count) {
                smallest = i;
            }
        }
        
        size_t second = (smallest == 0) ? 1 : 0;
        for (size_t i = 0; i < nodes.size(); i++) {
            if (i == smallest) continue;
            if (nodes[i].count < nodes[second].count) {
                second = i;
            }
        }

        Node parent(0, nodes[smallest].count + nodes[second].count, false);

        parent.children.push_back(nodes[smallest]);
        parent.children.push_back(nodes[second]);

        if (smallest > second) {
            std::swap(smallest, second);
        }
        nodes.erase(nodes.begin() + second);
        nodes.erase(nodes.begin() + smallest);
        nodes.push_back(parent);
    }

    return nodes[0];
}