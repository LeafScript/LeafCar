project(bsp_lib)

# project
set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(COMMON_DIR ${PROJ_DIR}/common)
set(SRC_DIR ${PROJ_DIR}/src)

# 3rd_party
set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(ARM_DIR ${3RD_DIR}/arm)
set(CM3_DIR ${ARM_DIR}/cmsis/cmsis_core/cm3)
set(ST_DIR ${3RD_DIR}/st)
set(ST_CMSIS_CORE_DIR ${ST_DIR}/cmsis/cmsis_core)
set(ST_SPL_INC_DIR ${ST_DIR}/cmsis/spl/inc)
set(ST_USER_DIR ${ST_DIR}/user_templete)
set(ST_USER_STM32F10X_DIR ${ST_USER_DIR}/stm32f10x)

# common
set(COMMON_SRC_DIR ${COMMON_DIR}/src)
set(COMMON_INC_DIR ${COMMON_DIR}/inc)

# src
set(PLAT_DIR ${SRC_DIR}/platform)
set(BSP_DIR ${SRC_DIR}/bsp)
set(DEBUG_DIR ${SRC_DIR}/debug)

# bsp
set(LED_DIR ${BSP_DIR}/led)
set(REDWIRE_DIR ${BSP_DIR}/redwire)
set(TB6612_DIR ${BSP_DIR}/tb6612)
set(TIMER_DIR ${BSP_DIR}/timer)
set(USART_DIR ${BSP_DIR}/usart)

include_directories(
    # 3rd_party
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${CM3_DIR}
    ${ST_CMSIS_CORE_DIR} ${ST_SPL_INC_DIR} ${ST_USER_STM32F10X_DIR}
    # common
    ${COMMON_INC_DIR}
    # platform
    ${PLAT_DIR}
    # bsp
    ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${USART_DIR}
    # debug
    ${DEBUG_DIR}
)

file(GLOB_RECURSE BSP_SRCS "${BSP_DIR}/*.c")
add_library(bsp_lib STATIC ${BSP_SRCS})
target_link_libraries(bsp_lib arm_lib st_lib alientek_lib)