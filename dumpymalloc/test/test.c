#include <stdlib.h>
#include <stdio.h>

int main() {
    void* p = malloc(512);
    printf("Malloced: %p\n", p);

    p = realloc(p, 1024);
    printf("Realloced: %p\n", p);
    free(p);

    p = calloc(512, sizeof(int));
    printf("Calloced: %p\n", p);
    free(p);
}
