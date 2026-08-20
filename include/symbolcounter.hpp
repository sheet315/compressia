#pragma once

#include <cstdint>
#include <array>

std::array<uint64_t, 256> countSymbols(std::string_view filebuffer);