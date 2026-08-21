#pragma once

#include <cstdint>
#include <array>

std::array<uint64_t, 256>                     countSymbols(std::string_view filebuffer);
std::array<std::pair<uint8_t, uint64_t>, 256> sortArray(std::array<uint64_t, 256>& symbolCounts);