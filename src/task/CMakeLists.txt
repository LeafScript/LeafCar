project(task_lib)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(INC_DIR ${SRC_DIR}/include)
set(COMMON_DIR ${SRC_DIR}/common)
set(BSP_DIR ${SRC_DIR}/bsp)
set(CONTROL_DIR ${SRC_DIR}/control)
set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(TASK_DIR ${SRC_DIR}/task)
set(CARTASK_DIR ${TASK_DIR}/cartask)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USART_DIR ${ALIENTEK_DIR}/system/usart)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

include_directories(${COMMON_DIR}
    ${CAR_DIR} ${PID_DIR} ${CARTASK_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USART_DIR} ${AT_USMART_DIR})

file(GLOB_RECURSE TASK_SRCS "${TASK_DIR}/*.c")
add_library(task_lib STATIC ${TASK_SRCS})