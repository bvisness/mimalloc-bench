#include <stdio.h>
#include <stdlib.h>

#include "common.h"

/* This test checks that pointers aren't immediately re-used between
 * allocations. */

int main(void) {
    void *p = malloc(8);
    void *q = p;
    free(p);

    p = malloc(8);

    if (p == q)
    {
        NOT_CAUGHT();
    }

    return 0;
}
