cmake_minimum_required(VERSION 3.10)
project(control_lib)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(INC_DIR ${SRC_DIR}/include)
set(3RD_DIR ${SRC_DIR}/3rd_party)
set(COMMON_DIR ${SRC_DIR}/common)
set(BSP_DIR ${SRC_DIR}/bsp)
# todo: delete after refactor
set(CONTROL_DIR ${SRC_DIR}/control)
set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(TASK_DIR ${SRC_DIR}/task)
set(CARTASK_DIR ${TASK_DIR}/cartask)

set(INC_BSP_DIR ${INC_DIR}/bsp)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USART_DIR ${ALIENTEK_DIR}/system/usart)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

set(K210_DIR ${BSP_DIR}/k210)
set(LED_DIR ${BSP_DIR}/led)
set(REDWIRE_DIR ${BSP_DIR}/redwire)
set(TB6612_DIR ${BSP_DIR}/tb6612)
set(TIMER_DIR ${BSP_DIR}/timer)
set(WIRELESS_DIR ${BSP_DIR}/wireless)

include_directories(
    ${K210_DIR} ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${WIRELESS_DIR}
    ${CAR_DIR} ${PID_DIR} ${CARTASK_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USART_DIR} ${AT_USMART_DIR})

file(GLOB_RECURSE CONTROL_SRCS "${CONTROL_DIR}/*.c")
add_library(control_lib STATIC ${CONTROL_SRCS})
target_link_libraries(control_lib bsp_lib)