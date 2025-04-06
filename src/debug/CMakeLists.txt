project(debug_lib)

# project
set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(COMMON_DIR ${PROJ_DIR}/common)
set(COMPONENT_DIR ${PROJ_DIR}/component)
set(SRC_DIR ${PROJ_DIR}/src)

# 3rd_party
set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)
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

# component
set(LEAFCLI_DIR ${COMPONENT_DIR}/leafcli)
set(TASK_GEN_DIR ${COMPONENT_DIR}/task_gen)
set(SERVICE_TIMER_DIR ${COMPONENT_DIR}/service_timer)

# src
set(PLAT_DIR ${SRC_DIR}/platform)
set(SERVICE_DIR ${SRC_DIR}/service)
set(CONTROL_DIR ${SRC_DIR}/control)
set(BSP_DIR ${SRC_DIR}/bsp)
set(DEBUG_DIR ${SRC_DIR}/debug)

# service
set(FOUNDATION_DIR ${SERVICE_DIR}/foundation)
set(TASK_DIR ${SERVICE_DIR}/task)
set(CAR_TASK_DIR ${TASK_DIR}/car_task)
set(DEBUG_TASK_DIR ${TASK_DIR}/debug_task)
set(UPDATE_TASK_DIR ${TASK_DIR}/update_task)

# control
set(CAR_DIR ${CONTROL_DIR}/car)
set(CARARM_DIR ${CONTROL_DIR}/arm)
set(MOTOR_DIR ${CONTROL_DIR}/motor)
set(K210_DIR ${CONTROL_DIR}/k210)
set(OPENMV_DIR ${CONTROL_DIR}/openmv)
set(PID_DIR ${CONTROL_DIR}/pid)

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
    # component
    ${LEAFCLI_DIR} ${TASK_GEN_DIR} ${SERVICE_TIMER_DIR}
    # platform
    ${PLAT_DIR}
    # service
    ${FOUNDATION_DIR}
    ${TASK_DIR} ${CAR_TASK_DIR} ${DEBUG_TASK_DIR} ${UPDATE_TASK_DIR}
    # control
    ${CAR_DIR} ${CARARM_DIR} ${MOTOR_DIR} ${K210_DIR} ${OPENMV_DIR} ${PID_DIR}
    # bsp
    ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${USART_DIR}
    # debug
    ${DEBUG_DIR}
)

file(GLOB_RECURSE DEBUG_SRCS "${DEBUG_DIR}/*.c")
add_library(debug_lib STATIC ${DEBUG_SRCS})