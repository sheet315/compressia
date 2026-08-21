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

    std::ifstream      file(argv[1], std::ios::binary);
    std::ostringstream ss;
    ss << file.rdbuf();

    std::string buffer = ss.str();

    file.seekg(0, std::ios::end);
    std::streamsize fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    file.close();

    Node                         tree;
    std::array<std::string, 256> codeLUT{};

    {
        std::array<uint64_t, 256>                     symbolCounts = countSymbols(buffer);
        std::array<std::pair<uint8_t, uint64_t>, 256> symbolPairs  = sortArray(symbolCounts);
        std::vector<Node>                             nodes        = genNodes(symbolPairs);
        tree = genTree(nodes);

        for (size_t i = 0; i < 256; i++) {
            if (symbolPairs[i].second == 0) continue;
            std::string code   = "";
            uint8_t     target = symbolPairs[i].first;

            if (!getCode(tree, target, code)) {
                std::cout << "some error happened while compressing data (could not find symbol in tree)\n";
                return 1;
            }

            codeLUT[target] = code; 
        }
    }

    std::vector<uint8_t> serialized = serializeTree(tree);
    std::vector<uint8_t> databytes;
    std::ofstream        out(argv[2], std::ios::binary);

    std::string header    = "COMPRESSIA 1.0.0";
    std::string treeEnd   = "TREE_END";
    std::string headerEnd = "HEAD";

    out.write(header.data(), header.size());
    out.write((const char*)serialized.data(), serialized.size());
    out.write(treeEnd.data(), treeEnd.size());
    
    int      count     = 0;
    uint8_t  byte      = 0;
    uint64_t bytecount = 0;
    size_t   index     = 0;

    while (index < buffer.size()) {
        char        c    = buffer[index];
        std::string &code = codeLUT[(uint8_t)c];

        for (size_t i = 0; i < code.size(); i++) {
            if (count == 8) {
                databytes.push_back(byte);
                count = 0;
                byte = 0;
                bytecount++;
            }

            byte <<= 1;
            byte |= (code[i] == '0' ? 0 : 1);

            count++;
        }

        if (index % 150000 == 0) printf("compressed %llu bytes. %.2f%% done\r", index, ((double)index / (double)fsize) * 100.0);

        index++;
    }

    if (count != 0) {
        byte <<= (8 - count);
        databytes.push_back(byte);
        bytecount++;
    }

    for (size_t i = 0; i < 8; i++) {
        out << ((bytecount >> (8 * i)) & 0xFF);
    }

    out.write(headerEnd.data(), headerEnd.size());
    out.write((const char*)databytes.data(), databytes.size());
}