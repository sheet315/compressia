#include <array>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "../include/symbolcounter.hpp"

std::array<uint64_t, 256> countSymbols(std::string_view filebuffer) {
    std::array<uint64_t, 256> symbolCounts{};

    for (size_t i = 0; i < filebuffer.size(); i++) {
        symbolCounts[(uint8_t)(filebuffer[i])]++;
    }
    
    return symbolCounts;
}

std::array<std::pair<uint8_t, uint64_t>, 256> sortArray(std::array<uint64_t, 256>& symbolCounts) {
    std::array<std::pair<uint8_t, uint64_t>, 256> sorted{};

    for (size_t i = 0; i < 256; i++) {
        sorted[i] = {(uint8_t)i, symbolCounts[i]};
    }

    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    return sorted;
}