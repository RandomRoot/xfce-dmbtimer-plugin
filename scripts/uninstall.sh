#!/bin/bash

[ "$UID" -eq 0 ] || exec sudo "$0" "$@"
cd build && make uninstall
