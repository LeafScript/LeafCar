file(GLOB_RECURSE SRCS "*.c")
add_library(service_leafcar_lib STATIC ${SRCS})
target_link_libraries(service_leafcar_lib PRIVATE
    project_common
    common_lib
    control_lib
    leafcli_lib
    task_gen_lib
)