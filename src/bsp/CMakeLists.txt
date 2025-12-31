project(bsp_lib)

file(GLOB_RECURSE BSP_SRCS "*.c")
add_library(bsp_lib STATIC ${BSP_SRCS})
target_link_libraries(bsp_lib PRIVATE
    project_common
    arm_lib
    st_lib
    alientek_lib
)