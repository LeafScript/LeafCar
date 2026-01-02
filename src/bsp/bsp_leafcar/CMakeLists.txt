file(GLOB_RECURSE BSP_SRCS "*.c")
add_library(bsp_leafcar_lib STATIC ${BSP_SRCS})
target_link_libraries(bsp_leafcar_lib PRIVATE
    project_common
)