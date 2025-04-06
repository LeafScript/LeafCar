project(control_lib)

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

# common
set(COMMON_SRC_DIR ${COMMON_DIR}/src)
set(COMMON_INC_DIR ${COMMON_DIR}/inc)

# component
set(SERVICE_TIMER_DIR ${COMPONENT_DIR}/service_timer)

# src
set(PLAT_DIR ${SRC_DIR}/platform)
set(CONTROL_DIR ${SRC_DIR}/control)
set(BSP_DIR ${SRC_DIR}/bsp)
set(DEBUG_DIR ${SRC_DIR}/debug)

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
    ${AT_SYS_DIR} ${AT_DELAY_DIR}
    # common
    ${COMMON_INC_DIR}
    # component
    ${SERVICE_TIMER_DIR}
    # platform
    ${PLAT_DIR}
    # control
    ${CAR_DIR} ${CARARM_DIR} ${MOTOR_DIR} ${K210_DIR} ${OPENMV_DIR} ${PID_DIR}
    # bsp
    ${LED_DIR} ${REDWIRE_DIR} ${TB6612_DIR} ${TIMER_DIR} ${USART_DIR}
    # debug
    ${DEBUG_DIR}
)

file(GLOB_RECURSE CONTROL_SRCS "${CONTROL_DIR}/*.c")
add_library(control_lib STATIC ${CONTROL_SRCS})
target_link_libraries(control_lib bsp_lib)