file(GLOB_RECURSE SRCS "*.c")
add_library(debug_standcar_lib STATIC ${SRCS})
target_link_libraries(debug_standcar_lib PRIVATE
    project_common
)