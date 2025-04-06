project(leafcli_lib)

set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(COMMON_INC_DIR ${PROJ_DIR}/common/inc)
set(LEAFCLI_DIR ${PROJ_DIR}/component/leafcli)

include_directories(${COMMON_INC_DIR} ${LEAFCLI_DIR})

add_library(leafcli_lib STATIC "${LEAFCLI_DIR}/leafcli.c")