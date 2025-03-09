#!/bin/bash

test_mode=$1
board_name=$2
test_case=$3

if [ "$test_mode" = "board" ]; then
    board_dir="../test/board_test/$board_name/$test_case"
    if [ ! -d "$board_dir" ]; then
        echo "Board directory not found: $board_dir"
        exit 1
    fi
    echo "Cleaning test project for board[$board_name] case[$test_case]"
    output_dir="$board_dir/output"
    rm -r "$output_dir"
    pwd=$(pwd)
else
    echo "Unsupport test mode: $test_mode"
    exit 1
fi