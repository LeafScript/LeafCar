cmake_minimum_required(VERSION 3.10)
project(3rd_lib)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(INC_DIR ${SRC_DIR}/include)
set(3RD_DIR ${SRC_DIR}/3rd_party)
# todo: delete after refactor
set(BSP_DIR ${SRC_DIR}/bsp)
set(LED_DIR ${BSP_DIR}/led)
set(K210_DIR ${BSP_DIR}/k210)
set(CONTROL_DIR ${SRC_DIR}/control)
set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(CARARM_DIR ${CONTROL_DIR}/arm)
set(TASK_DIR ${SRC_DIR}/task)
set(CARTASK_DIR ${TASK_DIR}/cartask)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USART_DIR ${ALIENTEK_DIR}/system/usart)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

set(ARM_DIR ${3RD_DIR}/arm)
set(CM3_DIR ${ARM_DIR}/cmsis/cmsis_core/cm3)

set(ST_DIR ${3RD_DIR}/st)
set(ST_CMSIS_CORE_DIR ${ST_DIR}/cmsis/cmsis_core)
set(ST_SPL_INC_DIR ${ST_DIR}/cmsis/spl/inc)
set(ST_USER_DIR ${ST_DIR}/user_templete)
set(ST_USER_STM32F10X_DIR ${ST_USER_DIR}/stm32f10x)

include_directories(${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USART_DIR} ${AT_USMART_DIR}
    ${CM3_DIR} ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR}
    ${LED_DIR} ${K210_DIR} ${CAR_DIR} ${CARTASK_DIR} ${PID_DIR} ${CARARM_DIR})

file(GLOB_RECURSE 3RD_SRCS "${3RD_DIR}/*.c")
add_library(3rd_lib STATIC ${3RD_SRCS})