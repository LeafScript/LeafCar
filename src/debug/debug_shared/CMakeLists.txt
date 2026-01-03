file(GLOB_RECURSE SRCS "*.c")
add_library(debug_shared_lib STATIC ${SRCS})
target_link_libraries(debug_shared_lib PRIVATE
    project_common
)