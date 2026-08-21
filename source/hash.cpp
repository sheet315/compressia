#include <cstdint>
#include <string>

#include "../include/hash.hpp"

Bucket buckets[BUCKETCOUNT];
Entry  entries[ENTRYCOUNT];

uint32_t hash(uint8_t a, uint8_t b, uint8_t c) {
    return ((a << 8) ^ (b << 4) ^ c) & 0xFFFF;
}

void addEntry(const std::string& data, size_t pos) {
    if (pos + 2 >= data.size()) {
        return;
    }

    uint32_t h = hash(data[pos], data[pos + 1], data[pos + 2]);

    uint32_t bucket = h % BUCKETCOUNT;
    uint32_t entry  = pos & (ENTRYCOUNT - 1);

    entries[entry].position = pos;
    entries[entry].next     = buckets[bucket].head;
    entries[entry].valid    = true;
    buckets[bucket].head    = entry;
}

Match findMatch(const std::string& data, size_t pos) {
    Match best{};

    if (pos + 2 >= data.size()) {
        return best;
    }

    uint32_t h = hash(data[pos], data[pos + 1], data[pos + 2]);
    uint32_t bucket = h % BUCKETCOUNT;
    uint32_t entry = buckets[bucket].head;
    size_t   attempts = 0;

    while (entry != UINT32_MAX && attempts < 4) {
        if (!entries[entry].valid || entries[entry].position >= pos) {
            break;
        }

        size_t candidate = entries[entry].position;

        if (pos - candidate > ENTRYCOUNT) {
            break;
        }

        size_t length = 0;
        while (length < 255 && pos + length < data.size() && data[candidate + length] == data[pos + length]) {
            length++;
        }

        if (length > best.length) {
            best.length = length;
            best.distance = pos - candidate;

            if (best.length == 255) {
                break;
            }
        }

        entry = entries[entry].next;
        attempts++;
    }

    return best;
}