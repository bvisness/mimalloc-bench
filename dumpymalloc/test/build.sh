#!/bin/bash

set -euo pipefail

gcc -Wl,--unresolved-symbols=ignore-all test.c
