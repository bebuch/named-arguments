#!/bin/bash

# exit script on any error
set -e
set -o xtrace

# Build examples
cd examples
mkdir -p build
cd build
cmake ..
make

# Run examples
./main
