#!/bin/bash
# A sample Bash script, by Ryan

rm -rf build
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../lib/vcpkg/scripts/buildsystems/vcpkg.cmake
cd build
make
cd ..

echo "Build complete!"