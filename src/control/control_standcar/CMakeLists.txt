file(GLOB_RECURSE SRCS "*.c")
add_library(control_standcar_lib STATIC ${SRCS})
target_link_libraries(control_standcar_lib PRIVATE
    project_common
)