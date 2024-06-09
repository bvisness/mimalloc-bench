#include <stdio.h>

#include "dumpyserialize.h"

int main(int argc, char** argv) {
    FILE* in = fopen(argv[1], "r");

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
                printf("aligned_alloc(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
            } break;
            case CALLOC: {
                size_t num;
                size_t size;
                void* p;
                readsize(&num, in);
                readsize(&size, in);
                readptr(&p, in);
                printf("calloc(num=%ld, size=%ld) = %p\n", num, size, p);
            } break;
            case FREE: {
                void* addr;
                readptr(&addr, in);
                printf("free(addr=%p)\n", addr);
            } break;
            case MALLOC: {
                size_t size;
                void* p;
                readsize(&size, in);
                readptr(&p, in);
                printf("malloc(size=%ld) = %p\n", size, p);
            } break;
            case MEMALIGN: {
                size_t alignment;
                size_t size;
                void* p;
                readsize(&alignment, in);
                readsize(&size, in);
                readptr(&p, in);
                printf("memalign(alignment=%ld, size=%ld) = %p\n", alignment, size, p);
            } break;
            case REALLOC: {
                void* ptr;
                size_t new_size;
                void* p;
                readptr(&ptr, in);
                readsize(&new_size, in);
                readptr(&p, in);
                printf("realloc(ptr=%p, new_size=%ld) = %p\n", ptr, new_size, p);
            } break;
        }
    }
}
