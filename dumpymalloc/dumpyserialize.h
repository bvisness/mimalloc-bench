#pragma once

#include <stdint.h>

enum DumpyCall {
    ALIGNED_ALLOC = 1,
    CALLOC = 2,
    FREE = 3,
    MALLOC = 4,
    MEMALIGN = 5,
    POSIX_MEMALIGN = 6,
    REALLOC = 7,
};

void writechar(char n, FILE* out) {
    if (!out) return;
    uint8_t n8 = (uint8_t)n;
    fwrite(&n8, sizeof(uint8_t), 1, out);
}

void write32(int n, FILE* out) {
    if (!out) return;
    uint32_t n64 = (uint32_t)n;
    fwrite(&n64, sizeof(uint32_t), 1, out);
}

void writesize(size_t n, FILE* out) {
    if (!out) return;
    uint64_t n64 = (uint64_t)n;
    fwrite(&n64, sizeof(uint64_t), 1, out);
}

void writeptr(void* p, FILE* out) {
    if (!out) return;
    uint64_t p64 = (uint64_t)p;
    fwrite(&p64, sizeof(uint64_t), 1, out);
}

void readsize(size_t* out, FILE* in) {
    fread(out, sizeof(size_t), 1, in);
}

void readptr(void** out, FILE* in) {
    fread(out, sizeof(void*), 1, in);
}
