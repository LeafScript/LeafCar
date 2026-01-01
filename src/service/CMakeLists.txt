project(service_lib)

file(GLOB_RECURSE SERVICE_SRCS "*.c")
add_library(service_lib STATIC ${SERVICE_SRCS})
target_link_libraries(service_lib PRIVATE
    project_common
    common_lib
    control_lib
    leafcli_lib
    task_gen_lib
)