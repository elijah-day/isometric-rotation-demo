#!/bin/bash

cmake CMakeLists.txt -B build -DCMAKE_TOOLCHAIN_FILE=WindowsToolchain.cmake
cmake --build build
cp -r data/ build/
cp ./external/SDL3-3.2.8/x86_64-w64-mingw32/bin/SDL3.dll build/
cp ./external/SDL3_image-3.2.4/x86_64-w64-mingw32/bin/SDL3_image.dll build/
cd build
wine main.exe
