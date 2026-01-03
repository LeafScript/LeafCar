file(GLOB_RECURSE SRCS "*.c")
add_library(debug_leafcar_lib STATIC ${SRCS})
target_link_libraries(debug_leafcar_lib PRIVATE
    project_common
)