#define _GNU_SOURCE

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dumpyserialize.h"

static void* (*real_aligned_alloc)(size_t, size_t) = NULL;
static void* (*real_calloc)(size_t, size_t) = NULL;
static void (*real_free)(void*) = NULL;
static void* (*real_malloc)(size_t) = NULL;
static void* (*real_memalign)(size_t, size_t) = NULL;
// static int (*real_posix_memalign)(void**, size_t, size_t) = NULL;
static void* (*real_realloc)(void*, size_t) = NULL;

#define DUMPY_LOAD(name) \
    real_##name = dlsym(RTLD_NEXT, #name); \
    if (!real_##name) { \
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror()); \
    }

#define OUTBUF_SIZE 2048
FILE* out = NULL;
char outbuf[OUTBUF_SIZE];

static void dumpy_init(void) {
    DUMPY_LOAD(aligned_alloc);
    DUMPY_LOAD(calloc);
    DUMPY_LOAD(free);
    DUMPY_LOAD(malloc);
    DUMPY_LOAD(memalign);
    // DUMPY_LOAD(posix_memalign);
    DUMPY_LOAD(realloc);

    char filename[1024];
    sprintf(filename, "dumpydump-%d", getpid());
    out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "Failed to open file: %d\n", errno);
    }
    setvbuf(out, outbuf, _IOFBF, OUTBUF_SIZE);
}

#define DUMPY_INIT(thing) if (!real_##thing) { dumpy_init(); }

void *aligned_alloc(size_t alignment, size_t size) {
    DUMPY_INIT(aligned_alloc);

    writechar(ALIGNED_ALLOC, out);
    void* p = real_aligned_alloc(alignment, size);
    writesize(alignment, out);
    writesize(size, out);
    writeptr(p, out);
    return p;
}

void* calloc(size_t num, size_t size) {
    DUMPY_INIT(calloc);

    writechar(CALLOC, out);
    void* p = real_calloc(num, size);
    writesize(num, out);
    writesize(size, out);
    writeptr(p, out);
    return p;
}

void free(void* addr) {
    DUMPY_INIT(free);

    writechar(FREE, out);
    real_free(addr);
    writeptr(addr, out);
}

void* malloc(size_t size) {
    DUMPY_INIT(malloc);

    writechar(MALLOC, out);
    void* p = real_malloc(size);
    writesize(size, out);
    writeptr(p, out);
    return p;
}

void* memalign(size_t alignment, size_t size) {
    DUMPY_INIT(memalign);

    writechar(MEMALIGN, out);
    void* p = real_memalign(alignment, size);
    writesize(alignment, out);
    writesize(size, out);
    writeptr(p, out);
    return p;
}

// int posix_memalign(void **memptr, size_t alignment, size_t size) {
//     DUMPY_INIT(posix_memalign);

//     writechar(POSIX_MEMALIGN, out);
//     int n = real_posix_memalign(memptr, alignment, size);
//     writeptr(memptr, out);
//     writesize(alignment, out);
//     writesize(size, out);
//     write32(n, out);
//     return n;
// }

void* realloc(void* ptr, size_t new_size) {
    DUMPY_INIT(realloc);

    writechar(REALLOC, out);
    void* p = real_realloc(ptr, new_size);
    writeptr(ptr, out);
    writesize(new_size, out);
    writeptr(p, out);
    return p;
}
