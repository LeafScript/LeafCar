project(debug_lib)

file(GLOB_RECURSE DEBUG_SRCS "*.c")
add_library(debug_lib STATIC ${DEBUG_SRCS})
target_link_libraries(debug_lib PRIVATE project_common)