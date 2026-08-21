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
        parent.children.push_back(nodes[second]  );

        if (smallest > second) {
            std::swap(smallest, second);
        }

        nodes.erase(nodes.begin() + second);
        nodes.erase(nodes.begin() + smallest);
        nodes.push_back(parent);
    }

    return nodes[0];
}

std::vector<uint8_t> serializeTree(Node& tree) {
    std::vector<uint8_t> data;

    if (tree.isLeaf) {
        data.push_back(1);
        data.push_back(tree.byte);
        return data;
    }

    data.push_back(0);

    std::vector<uint8_t> left  = serializeTree(tree.children[0]);
    std::vector<uint8_t> right = serializeTree(tree.children[1]);

    data.insert(data.end(), left.begin(),  left.end());
    data.insert(data.end(), right.begin(), right.end());

    return data;
}

bool getCode(Node& node, uint8_t target, std::string& code) {
    if (node.isLeaf) {
        return node.byte == target;
    }

    code.push_back('0');

    if (getCode(node.children[0], target, code)) {
        return true;
    }

    code.pop_back();
    code.push_back('1');

    if (getCode(node.children[1], target, code)) {
        return true;
    }

    code.pop_back();

    return false;
}