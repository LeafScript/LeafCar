cmake_minimum_required(VERSION 3.10)
project(leafcar)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O1 -mcpu=cortex-m3 -ffreestanding \
    --specs=nosys.specs -u _printf_float")

add_definitions(-DSTM32F10X_HD -DUSE_STDPERIPH_DRIVER)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(PLAT_DIR ${SRC_DIR}/platform)
set(CMAKE_EXE_LINKER_FLAGS "-T ${PLAT_DIR}/stm32f103c8t6.ld")
set(INC_DIR ${SRC_DIR}/include)
set(BSP_DIR ${SRC_DIR}/bsp)
set(COMMON_DIR ${SRC_DIR}/common)
set(CONTROL_DIR ${SRC_DIR}/control)
set(TASK_DIR ${SRC_DIR}/task)
set(DEBUG_DIR ${SRC_DIR}/debug)

set(CONTROL_DIR ${SRC_DIR}/control)
set(CAR_DIR ${CONTROL_DIR}/car)
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

set(K210_DIR ${BSP_DIR}/k210)
set(LED_DIR ${BSP_DIR}/led)
set(REDWIRE_DIR ${BSP_DIR}/redwire)
set(TB6612_DIR ${BSP_DIR}/tb6612)
set(TIMER_DIR ${BSP_DIR}/timer)
set(WIRELESS_DIR ${BSP_DIR}/wireless)

include_directories(${INC_DIR} ${TASK_DIR}
    ${K210_DIR} ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${WIRELESS_DIR}
    ${CAR_DIR} ${CARARM_DIR} ${CARTASK_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USART_DIR} ${AT_USMART_DIR}
    ${CM3_DIR} ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR})

file(GLOB_RECURSE ARM_SRCS "${ARM_DIR}/*.c")
add_library(arm_lib STATIC ${ARM_SRCS})
file(GLOB_RECURSE ST_SRCS "${ST_DIR}/*.c")
add_library(st_lib STATIC ${ST_SRCS})
file(GLOB_RECURSE ALIENTEK_SRCS "${ALIENTEK_DIR}/*.c")
add_library(alientek_lib STATIC ${ALIENTEK_SRCS})

add_subdirectory(${BSP_DIR} bsp_lib)
add_subdirectory(${CONTROL_DIR} control_lib)
add_subdirectory(${TASK_DIR} task_lib)
add_subdirectory(${DEBUG_DIR} debug_lib)

add_executable(leafcar ${SRC_DIR}/main.c)
target_link_libraries(leafcar arm_lib st_lib alientek_lib bsp_lib control_lib task_lib debug_lib)