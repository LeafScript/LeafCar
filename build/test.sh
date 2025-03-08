#!/bin/bash

test_mode=$1
board_name=$2

if [ "$test_mode" = "board" ]; then
    board_dir="../test/board_test/$board_name"
    if [ ! -d "$board_dir" ]; then
        echo "Board directory not found: $board_dir"
        exit 1
    fi
    echo "Building test project for board: $board_name"
    src_dir="$board_dir/src"
    output_dir="$board_dir/output"
    pwd=$(pwd)
    toolchain_cmake="$pwd/arm-none-eabi-toolchain.cmake"
    cmake -S "$src_dir" -B "$output_dir" -D CMAKE_TOOLCHAIN_FILE="$toolchain_cmake" -G "Unix Makefiles"
    cmake --build "$output_dir" -j8
else
    echo "Unsupport test mode: $test_mode"
    exit 1
fi