#!/bin/bash

# 定义参数列表，使用关联数组存储每个 board 对应的 case
declare -A BOARD_CASE_MAP
BOARD_CASE_MAP["stm32/stm32f103c8t6"]="led_test freertos_test"
BOARD_CASE_MAP["stm32/stm32f103zet6"]=""

# 提取所有的 board 选项
BOARD_OPTIONS=("${!BOARD_CASE_MAP[@]}")

# 补全函数
_leafcar_completion() {
    local cur prev opts selected_board
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    # 命令选项
    opts="-h --help build clean test -b --board -c --case"

    # 查找当前选择的 board
    local i
    for ((i = 0; i < COMP_CWORD; i++)); do
        if [[ "${COMP_WORDS[i]}" == "-b" || "${COMP_WORDS[i]}" == "--board" ]]; then
            selected_board="${COMP_WORDS[i+1]}"
            break
        fi
    done

    case "${prev}" in
        -b|--board)
            COMPREPLY=( $(compgen -W "${BOARD_OPTIONS[*]}" -- "${cur}") )
            return 0
            ;;
        -c|--case)
            if [[ -n "$selected_board" && -n "${BOARD_CASE_MAP[$selected_board]}" ]]; then
                COMPREPLY=( $(compgen -W "${BOARD_CASE_MAP[$selected_board]}" -- "${cur}") )
            else
                # 如果没有选择 board，不提供补全
                COMPREPLY=()
            fi
            return 0
            ;;
    esac

    # 补全命令和选项
    COMPREPLY=( $(compgen -W "${opts}" -- "${cur}") )
    return 0
}

# 注册补全函数
complete -F _leafcar_completion leafcar.sh