project(service_timer_lib)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(COMMON_SRC_DIR ${PROJ_DIR}/common/src)
set(COMMON_INC_DIR ${PROJ_DIR}/common/inc)
set(SERVICE_TIMER_DIR ${PROJ_DIR}/component/service_timer)

include_directories(${COMMON_INC_DIR} ${SERVICE_TIMER_DIR})

add_library(service_timer_lib STATIC "${SERVICE_TIMER_DIR}/service_timer.c" "${COMMON_SRC_DIR}/log/log.c")