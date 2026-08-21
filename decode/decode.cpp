#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include "../include/treegen.hpp"

static bool matchLiteral(const std::vector<uint8_t>& data, size_t& pos, const std::string& lit) {
    if (pos + lit.size() > data.size()) return false;
    for (size_t i = 0; i < lit.size(); i++) {
        if (data[pos + i] != (uint8_t)lit[i]) return false;
    }
    pos += lit.size();
    return true;
}

static Node parseTree(const std::vector<uint8_t>& data, size_t& pos) {
    uint8_t flag = data[pos++];

    if (flag == 1) {
        uint8_t byte = data[pos++];
        return Node(byte, 0, true);
    }

    Node node(0, 0, false);
    node.children.push_back(parseTree(data, pos));
    node.children.push_back(parseTree(data, pos));
    return node;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << argv[0] << " {input} {output}\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file) {
        std::cout << "file failed to open\n";
        return 1;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> raw(size);
    file.read((char*)raw.data(), size);
    file.close();

    size_t pos = 0;

    if (!matchLiteral(raw, pos, "COMPRESSIA 1.0.0")) {
        std::cout << "bad header\n";
        return 1;
    }

    Node tree = parseTree(raw, pos);

    if (!matchLiteral(raw, pos, "TREE_END")) {
        std::cout << "bad tree terminator\n";
        return 1;
    }

    uint64_t bytecount = 0;
    for (size_t i = 0; i < 8; i++) {
        bytecount |= ((uint64_t)raw[pos++]) << (8 * i);
    }

    uint64_t origSize = 0;
    for (size_t i = 0; i < 8; i++) {
        origSize |= ((uint64_t)raw[pos++]) << (8 * i);
    }

    if (!matchLiteral(raw, pos, "HEAD")) {
        std::cout << "bad head terminator\n";
        return 1;
    }

    uint64_t totalBits = (raw.size() - pos) * 8;
    uint64_t bitpos = 0;

    std::string buffer;

    while (buffer.size() < origSize) {
        Node* cur = &tree;

        while (!cur->isLeaf) {
            if (bitpos >= totalBits) {
                std::cout << "corrupt stream: ran out of bits mid-symbol\n";
                return 1;
            }

            size_t byteIndex = pos + (bitpos / 8);
            size_t bitIndex  = 7 - (bitpos % 8);
            uint8_t bit = (raw[byteIndex] >> bitIndex) & 1;

            cur = &cur->children[bit];
            bitpos++;
        }

        buffer.push_back((char)cur->byte);
    }

    std::vector<uint8_t> output;
    size_t i = 0;

    while (i < buffer.size()) {
        uint8_t flag = (uint8_t)buffer[i++];

        if (flag == 0) {
            if (i >= buffer.size()) break;
            output.push_back((uint8_t)buffer[i++]);
        } else {
            if (i + 2 >= buffer.size()) break;

            uint32_t distance = (uint8_t)buffer[i] | ((uint32_t)(uint8_t)buffer[i + 1] << 8);
            uint32_t length   = (uint8_t)buffer[i + 2];
            i += 3;

            if (distance == 0 || distance > output.size()) {
                std::cout << "corrupt stream: bad match distance\n";
                return 1;
            }

            size_t start = output.size() - distance;
            for (uint32_t j = 0; j < length; j++) {
                output.push_back(output[start + j]);
            }
        }
    }

    std::ofstream out(argv[2], std::ios::binary);
    out.write((const char*)output.data(), (std::streamsize)output.size());

    std::cout << "decoded " << output.size() << " bytes\n";

    return 0;
}