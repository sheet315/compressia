#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../include/symbolcounter.hpp"
#include "../include/treegen.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << argv[0] << " {input} {output}\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();

    std::string buffer = ss.str();
    file.close();

    Node tree;

    {
        std::array<uint64_t, 256> symbolCounts = countSymbols(buffer);
        std::array<std::pair<uint8_t, uint64_t>, 256> symbolPairs = sortArray(symbolCounts);
        

        std::vector<Node> nodes = genNodes(symbolPairs);
        tree = genTree(nodes);
    }

    std::vector<uint8_t> serialized = serializeTree(tree);
    std::ofstream out(argv[2], std::ios::binary);

    std::string header = "COMPRESSIA 1.0.0";
    std::string magic = "COMP";
    out.write(header.data(), header.size());
    out.write((const char*)serialized.data(), serialized.size());
    out.write(magic.data(), magic.size());
    
}