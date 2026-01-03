file(GLOB_RECURSE SRCS "*.c")
add_library(control_leafcar_lib STATIC ${SRCS})
target_link_libraries(control_leafcar_lib PRIVATE
    project_common
    control_shared_lib
    service_timer_lib
    bsp_lib
)