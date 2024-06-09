#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>

static void* (*real_aligned_alloc)(size_t, size_t) = NULL;
static void* (*real_calloc)(size_t, size_t) = NULL;
static void (*real_free)(void*) = NULL;
static void* (*real_malloc)(size_t) = NULL;
static void* (*real_memalign)(size_t, size_t) = NULL;
static int (*real_posix_memalign)(void**, size_t, size_t) = NULL;
static void* (*real_realloc)(void*, size_t) = NULL;

#define DUMPY_LOAD(name) \
    real_##name = dlsym(RTLD_NEXT, #name); \
    if (!real_##name) { \
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror()); \
    }

int dumpy_initialized = 0;

static void dumpy_init(void) {
    DUMPY_LOAD(aligned_alloc);
    DUMPY_LOAD(calloc);
    DUMPY_LOAD(free);
    DUMPY_LOAD(malloc);
    DUMPY_LOAD(memalign);
    DUMPY_LOAD(posix_memalign);
    DUMPY_LOAD(realloc);

    dumpy_initialized = 1;
}

#define DUMPY_INIT() if (!dumpy_initialized) { dumpy_init(); }

void *aligned_alloc(size_t alignment, size_t size) {
    DUMPY_INIT();

    void* p = real_aligned_alloc(alignment, size);
    fprintf(stderr, "aligned_alloc(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
    return p;
}

void* calloc(size_t num, size_t size) {
    DUMPY_INIT();

    void* p = real_calloc(num, size);
    fprintf(stderr, "calloc(num=%ld, size=%ld) = %p\n", num, size, p);
    return p;
}

void free(void* addr) {
    DUMPY_INIT();

    fprintf(stderr, "free(%p)\n", addr);
    real_free(addr);
}

void* malloc(size_t size) {
    DUMPY_INIT();

    void* p = real_malloc(size);
    fprintf(stderr, "malloc(%ld) = %p\n", size, p);
    return p;
}

void* memalign(size_t alignment, size_t size) {
    DUMPY_INIT();

    void* p = real_memalign(alignment, size);
    fprintf(stderr, "memalign(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
    return p;
}

int posix_memalign(void **memptr, size_t alignment, size_t size) {
    DUMPY_INIT();

    int n = real_posix_memalign(memptr, alignment, size);
    fprintf(stderr, "posix_memalign(memptr=%p, alignment=%ld, size=%ld) = %d\n", memptr, alignment, size, n);
    return n;
}

void* realloc(void* ptr, size_t new_size) {
    DUMPY_INIT();

    void* p = real_realloc(ptr, new_size);
    fprintf(stderr, "realloc(ptr=%p, new_size=%ld) = %p\n", ptr, new_size, p);
    return p;
}
