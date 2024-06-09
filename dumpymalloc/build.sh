#!/bin/bash

set -euo pipefail

gcc -shared -o libdumpymalloc.so -fPIC dumpymalloc.c
nm -gD libdumpymalloc.so > libdumpymalloc.syms.txt

mkdir -p ../extern/dumpy
cp libdumpymalloc.so ../extern/dumpy/libdumpymalloc.so

gcc -o dumpytext dumpytext.c
