#!/bin/bash

cmake CMakeLists.txt -B build
cmake --build build
cp -r data/ build/
cd build
./main
