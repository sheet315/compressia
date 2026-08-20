#include <array>
#include <string>

#include "../include/symbolcounter.hpp"

std::array<uint64_t, 256> symbolCounts;

void countSymbols(std::string_view filebuffer) {
    for (size_t i = 0; i < filebuffer.size(); i++) {
        symbolCounts[(uint8_t)(filebuffer[i])]++;
    }
}