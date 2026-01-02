#!/bin/bash

product="$1"

# if product is not leafcar or standcar, exit
if [ "$product" != "leafcar" ] && [ "$product" != "standcar" ]; then
    echo "Unsupport product: $product"
    exit 1
fi

src_dir="../src"
output_dir="$src_dir/output/$product"
pwd=$(pwd)
echo "Building $product project, output dir: $pwd/$output_dir"
toolchain_cmake="$pwd/arm-none-eabi-toolchain.cmake"
cmake -S "$src_dir" -B "$output_dir" \
    -D CMAKE_TOOLCHAIN_FILE="$toolchain_cmake" \
    -D PRODUCT="$product" -G "Unix Makefiles"
cmake --build "$output_dir" -j8