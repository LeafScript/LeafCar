#!/bin/bash

src_dir="../src"
output_dir="$src_dir/output"
pwd=$(pwd)
toolchain_cmake="$pwd/arm-none-eabi-toolchain.cmake"
cmake -S "$src_dir" -B "$output_dir" -D CMAKE_TOOLCHAIN_FILE="$toolchain_cmake" -G "Unix Makefiles"
cmake --build "$output_dir" -j8