#pragma once

#include <cstdint>

#define BUCKETCOUNT 65536
#define ENTRYCOUNT  1048576

struct Entry {
    uint32_t position;
    uint32_t next;
    bool     valid = false;
};

struct Bucket {
    uint32_t head;
};

struct Match {
    size_t distance = 0;
    size_t length   = 0;
};

extern Bucket buckets[BUCKETCOUNT];
extern Entry entries[ENTRYCOUNT];

uint32_t hash(uint8_t a, uint8_t b, uint8_t c);
void     addEntry(const std::string& data, size_t pos);
Match    findMatch(const std::string& data, size_t pos);