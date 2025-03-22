project(service_lib)

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/..)
set(3RD_DIR ${PROJ_DIR}/3rd_party)
set(SRC_DIR ${PROJ_DIR}/src)
set(INC_DIR ${SRC_DIR}/include)
set(COMMON_DIR ${SRC_DIR}/common)
set(CONTROL_DIR ${SRC_DIR}/control)
set(CAR_DIR ${CONTROL_DIR}/car)
set(PID_DIR ${CONTROL_DIR}/pid)
set(SERVICE_DIR ${SRC_DIR}/service)
set(FOUNDATION_DIR ${SERVICE_DIR}/foundation)
set(TASK_DIR ${SERVICE_DIR}/task)
set(CAR_TASK_DIR ${TASK_DIR}/car_task)
set(UPDATE_TASK_DIR ${TASK_DIR}/update_task)

set(ALIENTEK_DIR ${3RD_DIR}/alientek)
set(AT_SYS_DIR ${ALIENTEK_DIR}/system/sys)
set(AT_DELAY_DIR ${ALIENTEK_DIR}/system/delay)
set(AT_USMART_DIR ${ALIENTEK_DIR}/usmart)

include_directories(${COMMON_DIR}
    ${CAR_DIR} ${PID_DIR} ${CAR_TASK_DIR} ${UPDATE_TASK_DIR} ${FOUNDATION_DIR}
    ${AT_SYS_DIR} ${AT_DELAY_DIR} ${AT_USMART_DIR})

file(GLOB_RECURSE SERVICE_SRCS "${SERVICE_DIR}/*.c")
add_library(service_lib STATIC ${SERVICE_SRCS})
target_link_libraries(service_lib control_lib)