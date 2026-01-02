add_subdirectory(control_leafcar)
set(dep "control_leafcar_lib")

file(GLOB_RECURSE CONTROL_SRCS "*.c")
add_library(control_lib STATIC ${CONTROL_SRCS})
target_link_libraries(control_lib PRIVATE
    project_common
    ${dep}
)