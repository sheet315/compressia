#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "../include/symbolcounter.hpp"

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

  
    std::array<uint64_t, 256> symbolCounts = countSymbols(buffer);
    std::array<std::pair<uint8_t, uint64_t>, 256> symbolPairs = sortArray(symbolCounts);

    for (size_t i = 0; i < 256; i++) {
        printf("[%2X] : %llu\n", symbolPairs[i].first, symbolPairs[i].second);
    }
}