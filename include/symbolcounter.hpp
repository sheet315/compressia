#pragma once

#include <cstdint>
#include <array>

extern std::array<uint64_t, 256> symbolCounts;
void countSymbols(std::string_view filebuffer);