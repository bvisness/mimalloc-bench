#pragma once

#include <stdint.h>
#include <stdio.h>

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

typedef struct DumpyFuncs {
    void (*aligned_alloc)(size_t, size_t, void*);
    void (*calloc)(size_t, size_t, void*);
    void (*free)(void*);
    void (*malloc)(size_t, void*);
    void (*memalign)(size_t, size_t, void*);
    void (*realloc)(void*, size_t, void*);
} DumpyFuncs;

void dumpyread(FILE* in, DumpyFuncs funcs) {
    while (1) {
        int kind = fgetc(in);
        if (kind == EOF) {
            break;
        }

        switch (kind) {
            case ALIGNED_ALLOC: {
                size_t alignment;
                size_t size;
                void* p;
                readsize(&alignment, in);
                readsize(&size, in);
                readptr(&p, in);
                if (funcs.aligned_alloc) {
                    funcs.aligned_alloc(alignment, size, p);
                }
            } break;
            case CALLOC: {
                size_t num;
                size_t size;
                void* p;
                readsize(&num, in);
                readsize(&size, in);
                readptr(&p, in);
                if (funcs.calloc) {
                    funcs.calloc(num, size, p);
                }
            } break;
            case FREE: {
                void* addr;
                readptr(&addr, in);
                if (funcs.free) {
                    funcs.free(addr);
                }
            } break;
            case MALLOC: {
                size_t size;
                void* p;
                readsize(&size, in);
                readptr(&p, in);
                if (funcs.malloc) {
                    funcs.malloc(size, p);
                }
            } break;
            case MEMALIGN: {
                size_t alignment;
                size_t size;
                void* p;
                readsize(&alignment, in);
                readsize(&size, in);
                readptr(&p, in);
                if (funcs.memalign) {
                    funcs.memalign(alignment, size, p);
                }
            } break;
            case REALLOC: {
                void* ptr;
                size_t new_size;
                void* p;
                readptr(&ptr, in);
                readsize(&new_size, in);
                readptr(&p, in);
                if (funcs.realloc) {
                    funcs.realloc(ptr, new_size, p);
                }
            } break;
        }
    }
}
