#include <stdio.h>
#include <stdlib.h>

#include "dumpyserialize.h"

// CSV

void csv_on_calloc(size_t num, size_t size, void* p) {
    printf("calloc,%ld\n", num * size);
}

void csv_on_malloc(size_t size, void* p) {
    printf("malloc,%ld\n", size);
}

void csv_on_realloc(void* ptr, size_t new_size, void* p) {
    printf("realloc,%ld\n", new_size);
}

void csv(FILE* in) {
    printf("type,size\n");
    dumpyread(in, (DumpyFuncs){
        .calloc = csv_on_calloc,
        .malloc = csv_on_malloc,
        .realloc = csv_on_realloc,
    });
}

// Linear histogram

#define LIN_BUCKET_SIZE 128
#define LIN_NBUCKETS 8192

size_t linbuckets[LIN_NBUCKETS];

size_t linbucket(size_t v) {
    return v / LIN_BUCKET_SIZE;
}

void lintrack(size_t v) {
    if (v >= LIN_BUCKET_SIZE * LIN_NBUCKETS) {
        fprintf(stderr, "WARNING: allocation size %ld bigger than max size %d\n", v, LIN_BUCKET_SIZE * LIN_NBUCKETS);
        return;
    }
    linbuckets[linbucket(v)]++;
}

void linhist_on_calloc(size_t num, size_t size, void* p) {
    lintrack(size);
}

void linhist_on_malloc(size_t size, void* p) {
    lintrack(size);
}

void linhist_on_realloc(void* ptr, size_t new_size, void* p) {
    lintrack(new_size);
}

void linhist(FILE* in) {
    dumpyread(in, (DumpyFuncs){
        .calloc = linhist_on_calloc,
        .malloc = linhist_on_malloc,
        .realloc = linhist_on_realloc,
    });
    printf("bucket,count\n");
    for (size_t i = 0; i < LIN_NBUCKETS; i++) {
        printf("%ld,%ld\n", i * LIN_BUCKET_SIZE, linbuckets[i]);
    }
}

// Power of 2 histogram

#define LOG_NBUCKETS 64

size_t logbuckets[LOG_NBUCKETS];

size_t log2s(size_t v) {
    size_t l = 0;
    while (v >>= 1) { ++l; }
    return l;
}

void logtrack(size_t v) {
    logbuckets[log2s(v)]++;
}

void loghist_on_calloc(size_t num, size_t size, void* p) {
    logtrack(size);
}

void loghist_on_malloc(size_t size, void* p) {
    logtrack(size);
}

void loghist_on_realloc(void* ptr, size_t new_size, void* p) {
    logtrack(new_size);
}

void loghist(FILE* in) {
    dumpyread(in, (DumpyFuncs){
        .calloc = loghist_on_calloc,
        .malloc = loghist_on_malloc,
        .realloc = loghist_on_realloc,
    });
    printf("bucket,count\n");
    for (size_t i = 0; i < LOG_NBUCKETS; i++) {
        printf("%lu,%ld\n", 1ul << i, logbuckets[i]);
    }
}

int main(int argc, char** argv) {
    FILE* in = fopen(argv[1], "r");
    if (!in) {
        fprintf(stderr, "ERROR: couldn't open file %s\n", argv[1]);
        exit(1);
    }

    // csv(in);
    // linhist(in);
    loghist(in);
}
