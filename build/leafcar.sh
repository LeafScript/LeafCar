#!/bin/bash

show_help() {
    echo "Usage: $0 <command> [options]"
    echo ""
    echo "command & options:"
    echo "  -h, --help          Show this help message and exit."
    echo "  build               Build project."
    echo "  clean               Clean project."
    echo "  test                Test project."
    echo "    -b, --board [dir]  Build test project for specified board."
    echo "    -c, --case [case]  Choose test case."
}

command=""
test_mode=""
board_name=""
test_case=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -b|--board)
            test_mode="board"
            board_name="$2"
            shift
            ;;
        -c|--case)
            test_case="$2"
            shift
            ;;
        *)
            command="$1"
            ;;
    esac
    shift
done

case $command in
    build)
        echo "===== Building project ====="
        ./build.sh
        ;;
    clean)
        echo "===== Cleaning project ====="
        ./clean.sh $test_mode $board_name $test_case
        ;;
    test)
        echo "===== Running Test ====="
        ./test.sh $test_mode $board_name $test_case
        ;;
    *)
        echo "Unknown command: '$command'"
        show_help
        exit 1
        ;;
esac