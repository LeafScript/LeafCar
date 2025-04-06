project(service_lib)

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
set(LEAFCLI_DIR ${COMPONENT_DIR}/leafcli)
set(TASK_GEN_DIR ${COMPONENT_DIR}/task_gen)
set(SERVICE_TIMER_DIR ${COMPONENT_DIR}/service_timer)

# src
set(PLAT_DIR ${SRC_DIR}/platform)
set(SERVICE_DIR ${SRC_DIR}/service)
set(CONTROL_DIR ${SRC_DIR}/control)
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

include_directories(
    # 3rd_party
    ${AT_SYS_DIR} ${AT_DELAY_DIR}
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
    # debug
    ${DEBUG_DIR}
)

file(GLOB_RECURSE SERVICE_SRCS "${SERVICE_DIR}/*.c")
add_library(service_lib STATIC ${SERVICE_SRCS})
target_link_libraries(service_lib control_lib leafcli_lib task_gen_lib)