#!/bin/bash

show_help() {
    echo "Usage: $0 [options] <command>"
    echo ""
    echo "Options:"
    echo "  -h, --help          Show this help message and exit."
    echo ""
    echo "Commands:"
    echo "  build              Build project."
    echo "  clean              Clean project."
    echo "  llt                Run LLT."
    echo "  hlt                Run HLT."
}

command=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
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
        ./clean.sh
        ;;
    llt)
        echo "===== Running LLT ====="
        ;;
    hlt)
        echo "===== Running HLT: [$demo_name] ====="
        ;;
    *)
        echo "Unknown command: '$command'"
        show_help
        exit 1
        ;;
esac