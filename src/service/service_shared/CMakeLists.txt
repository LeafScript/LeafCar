file(GLOB_RECURSE SRCS "*.c")
add_library(service_shared_lib STATIC ${SRCS})
target_link_libraries(service_shared_lib PRIVATE
    project_common
)