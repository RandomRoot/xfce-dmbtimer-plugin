#!/bin/bash

[ "$UID" -eq 0 ] || exec sudo "$0" "$@"

mkdir -p "build" && cd build && \
cmake -DCMAKE_INSTALL_PREFIX=/usr .. && \
make
make install
