file(GLOB_RECURSE CONTROL_SRCS "*.c")
add_library(control_shared_lib STATIC ${CONTROL_SRCS})
target_link_libraries(control_shared_lib PRIVATE
    project_common
    st_lib
)