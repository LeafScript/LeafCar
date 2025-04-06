project(task_gen_lib)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(COMMON_INC_DIR ${PROJ_DIR}/common/inc)
set(TASK_GEN_DIR ${PROJ_DIR}/component/task_gen)

include_directories(${COMMON_INC_DIR} ${TASK_GEN_DIR})

add_library(task_gen_lib STATIC "${TASK_GEN_DIR}/task_gen.c")