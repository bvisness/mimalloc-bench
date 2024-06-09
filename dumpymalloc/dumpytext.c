#include <stdio.h>

#include "dumpyserialize.h"

void on_aligned_alloc(size_t alignment, size_t size, void* p) {
    printf("aligned_alloc(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
}

void on_calloc(size_t num, size_t size, void* p) {
    printf("calloc(num=%ld, size=%ld) = %p\n", num, size, p);
}

void on_free(void* addr) {
    printf("free(addr=%p)\n", addr);
}

void on_malloc(size_t size, void* p) {
    printf("malloc(size=%ld) = %p\n", size, p);
}

void on_memalign(size_t alignment, size_t size, void* p) {
    printf("memalign(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
}

void on_realloc(void* ptr, size_t new_size, void* p) {
    printf("realloc(ptr=%p, new_size=%ld) = %p\n", ptr, new_size, p);
}

int main(int argc, char** argv) {
    FILE* in = fopen(argv[1], "r");

    dumpyread(in, (DumpyFuncs){
        .aligned_alloc = on_aligned_alloc,
        .calloc = on_calloc,
        .free = on_free,
        .malloc = on_malloc,
        .memalign = on_memalign,
        .realloc = on_realloc,
    });
}
