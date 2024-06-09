#!/bin/bash

set -euo pipefail

./build.sh

# echo "====== DEFAULT BEHAVIOR ======"
# ./a.out

echo "====== OVERRIDDEN BEHAVIOR ======"
LD_PRELOAD=../libdumpymalloc.so ./a.out
